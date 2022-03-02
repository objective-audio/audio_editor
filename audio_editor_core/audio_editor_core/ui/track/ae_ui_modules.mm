//
//  ae_ui_modules.mm
//

#include "ae_ui_modules.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_module_waveforms_presenter.h>
#include <audio_editor_core/ae_modules_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_modules_constants {
static std::size_t const reserving_interval = 100;
}

std::shared_ptr<ui_modules> ui_modules::make_shared(ui_project_id const &project_id) {
    auto const &app_level = app_level::global();
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);

    auto const location_pool = module_location_pool::make_shared();
    auto const modules_presenter =
        modules_presenter::make_shared(project_id.identifier, ui_root_level->display_space, location_pool);
    auto const waveforms = ui_module_waveforms::make_shared(project_id, location_pool);
    auto const &color = app_level->color;
    return std::shared_ptr<ui_modules>(new ui_modules{modules_presenter, ui_root_level->standard, color, waveforms});
}

ui_modules::ui_modules(std::shared_ptr<modules_presenter> const &presenter,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ae::color> const &color,
                       std::shared_ptr<ui_module_waveforms> const &waveforms)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _color(color),
      _waveforms(waveforms),
      _triangle_node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _triangle_mesh(ui::mesh::make_shared({.use_mesh_color = false}, nullptr, nullptr, nullptr)),
      _line_mesh(ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, nullptr, nullptr, nullptr)) {
    this->node->add_sub_node(this->_triangle_node);
    this->node->add_sub_node(this->_waveforms->node());
    this->node->add_sub_node(this->_line_node);

    this->_triangle_node->set_mesh(this->_triangle_mesh);
    this->_line_node->set_mesh(this->_line_mesh);

    this->_set_rect_count(0);

    presenter
        ->observe_locations([this](module_location_pool_event const &event) {
            switch (event.type) {
                case module_location_pool_event_type::fetched:
                case module_location_pool_event_type::replaced:
                    this->set_locations(event.elements);
                    break;
                case module_location_pool_event_type::updated:
                    this->update_locations(event.elements.size(), event.erased, event.inserted);
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);

    standard->renderer()
        ->observe_will_render([this](auto const &) { this->_presenter->update_if_needed(); })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const &color = this->_color;
            this->_line_node->set_color(color->module_frame());
            this->_triangle_node->set_color(color->module_bg());
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_modules::set_scale(ui::size const &scale) {
    this->_triangle_node->set_scale(scale);
    this->_line_node->set_scale(scale);
    this->_waveforms->set_scale(scale);
}

void ui_modules::set_locations(std::vector<std::optional<module_location>> const &locations) {
    this->_set_rect_count(locations.size());

    this->_vertex_data->write([&locations](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

        auto each = make_fast_each(locations.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &location = locations.at(idx);

            if (location.has_value()) {
                auto const &value = location.value();
                vertex_rects[idx].set_position(ui::region{.origin = {.x = value.x(), .y = -0.5f},
                                                          .size = {.width = value.width(), .height = 1.0f}});
            } else {
                vertex_rects[idx].set_position(ui::region::zero());
            }
        }
    });
}

void ui_modules::update_locations(std::size_t const count,
                                  std::vector<std::pair<std::size_t, module_location>> const &erased,
                                  std::vector<std::pair<std::size_t, module_location>> const &inserted) {
    this->_set_rect_count(count);

    this->_vertex_data->write([&erased, &inserted](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

        for (auto const &pair : erased) {
            vertex_rects[pair.first].set_position(ui::region::zero());
        }

        for (auto const &pair : inserted) {
            auto const &value = pair.second;
            vertex_rects[pair.first].set_position(
                ui::region{.origin = {.x = value.x(), .y = -0.5f}, .size = {.width = value.width(), .height = 1.0f}});
        }
    });
}

void ui_modules::_remake_data_if_needed(std::size_t const max_count) {
    if (max_count <= this->_remaked_count &&
        (this->_vertex_data != nullptr || this->_triangle_index_data != nullptr || this->_line_index_data != nullptr)) {
        return;
    }

    this->_vertex_data = nullptr;
    this->_triangle_index_data = nullptr;
    this->_line_index_data = nullptr;
    this->_triangle_mesh->set_vertex_data(nullptr);
    this->_triangle_mesh->set_index_data(nullptr);
    this->_line_mesh->set_vertex_data(nullptr);
    this->_line_mesh->set_index_data(nullptr);

    this->_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(max_count * vertex2d_rect::vector_count);
    this->_triangle_index_data =
        ui::dynamic_mesh_index_data::make_shared(max_count * triangle_index2d_rect::vector_count);
    this->_line_index_data = ui::dynamic_mesh_index_data::make_shared(max_count * line_index2d_rect::vector_count);

    this->_triangle_index_data->write([&max_count](std::vector<ui::index2d_t> &indices) {
        auto *index_rects = (triangle_index2d_rect *)indices.data();

        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * vertex2d_rect::vector_count);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });

    this->_line_index_data->write([&max_count](std::vector<ui::index2d_t> &indices) {
        auto *index_rects = (line_index2d_rect *)indices.data();

        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * vertex2d_rect::vector_count);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });

    this->_triangle_mesh->set_vertex_data(this->_vertex_data);
    this->_triangle_mesh->set_index_data(this->_triangle_index_data);
    this->_line_mesh->set_vertex_data(this->_vertex_data);
    this->_line_mesh->set_index_data(this->_line_index_data);

    this->_remaked_count = max_count;
}

void ui_modules::_set_rect_count(std::size_t const rect_count) {
    this->_remake_data_if_needed(common_utils::reserving_count(rect_count, ui_modules_constants::reserving_interval));

    if (this->_vertex_data) {
        this->_vertex_data->set_count(rect_count * vertex2d_rect::vector_count);
    }

    if (this->_triangle_index_data) {
        this->_triangle_index_data->set_count(rect_count * triangle_index2d_rect::vector_count);
    }

    if (this->_line_index_data) {
        this->_line_index_data->set_count(rect_count * line_index2d_rect::vector_count);
    }
}
