//
//  ae_ui_pasting_modules.mm
//

#include "ae_ui_pasting_modules.hpp"

#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_types.h>
#include <audio_editor_core/ae_pasting_modules_presenter.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_pasting_modules_constants {
static std::size_t const reserving_interval = 100;
}

std::shared_ptr<ui_pasting_modules> ui_pasting_modules::make_shared(window_lifetime_id const &window_lifetime_id,
                                                                    std::shared_ptr<ui::node> const &node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    auto const modules_presenter =
        pasting_modules_presenter::make_shared(window_lifetime_id, resource_lifetime->display_space);
    return std::make_shared<ui_pasting_modules>(modules_presenter, resource_lifetime->standard, node,
                                                app_lifetime->color.get());
}

ui_pasting_modules::ui_pasting_modules(std::shared_ptr<pasting_modules_presenter> const &presenter,
                                       std::shared_ptr<ui::standard> const &standard,
                                       std::shared_ptr<ui::node> const &node, ae::color *color)
    : _presenter(presenter),
      _color(color),
      _frame_node(ui::node::make_shared()),
      _frame_mesh(ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, nullptr, nullptr, nullptr)) {
    node->add_sub_node(this->_frame_node);

    this->_frame_node->set_mesh(this->_frame_mesh);

    this->_set_rect_count(0);

    presenter
        ->observe_contents([this](pasting_module_content_pool_event const &event) {
            switch (event.type) {
                case pasting_module_content_pool_event_type::fetched:
                case pasting_module_content_pool_event_type::replaced:
                    this->_set_contents(event.elements);
                    break;
                case pasting_module_content_pool_event_type::updated:
                    this->_update_contents(event.elements.size(), event.inserted, event.replaced, event.erased);
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
        ->observe_appearance(
            [this](auto const &) { this->_frame_node->set_color(this->_color->pasting_module_frame()); })
        .sync()
        ->add_to(this->_pool);
}

void ui_pasting_modules::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_frame_node->set_scale(scale);
}

void ui_pasting_modules::_set_contents(std::vector<std::optional<pasting_module_content>> const &contents) {
    this->_set_rect_count(contents.size());

    this->_vertex_data->write([&contents](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

        auto each = make_fast_each(contents.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &content = contents.at(idx);

            if (content.has_value()) {
                auto const &value = content.value();
                ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                        .size = {.width = value.width(), .height = 1.0f}};
                vertex_rects[idx].set_position(region);
            } else {
                vertex_rects[idx].set_position(ui::region::zero());
            }
        }
    });
}

void ui_pasting_modules::_update_contents(std::size_t const count,
                                          std::vector<std::pair<std::size_t, pasting_module_content>> const &inserted,
                                          std::vector<std::pair<std::size_t, pasting_module_content>> const &replaced,
                                          std::vector<std::pair<std::size_t, pasting_module_content>> const &erased) {
    this->_set_rect_count(count);

    this->_vertex_data->write([&erased, &inserted](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

        for (auto const &pair : erased) {
            vertex_rects[pair.first].set_position(ui::region::zero());
        }

        for (auto const &pair : inserted) {
            auto const &value = pair.second;
            ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                    .size = {.width = value.width(), .height = 1.0f}};

            vertex_rects[pair.first].set_position(region);
        }
    });
}

void ui_pasting_modules::_remake_data_if_needed(std::size_t const max_count) {
    if (max_count <= this->_remaked_count && (this->_vertex_data != nullptr || this->_frame_index_data != nullptr)) {
        return;
    }

    this->_vertex_data = nullptr;
    this->_frame_index_data = nullptr;
    this->_frame_mesh->set_vertex_data(nullptr);
    this->_frame_mesh->set_index_data(nullptr);

    this->_vertex_data = ui::dynamic_mesh_vertex_data::make_shared(max_count * vertex2d_rect::vector_count);
    this->_frame_index_data = ui::dynamic_mesh_index_data::make_shared(max_count * frame_index2d_rect::vector_count);

    this->_frame_index_data->write([&max_count](std::vector<ui::index2d_t> &indices) {
        auto *index_rects = (frame_index2d_rect *)indices.data();

        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * vertex2d_rect::vector_count);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });

    this->_frame_mesh->set_vertex_data(this->_vertex_data);
    this->_frame_mesh->set_index_data(this->_frame_index_data);

    this->_remaked_count = max_count;
}

void ui_pasting_modules::_set_rect_count(std::size_t const rect_count) {
    this->_remake_data_if_needed(
        common_utils::reserving_count(rect_count, ui_pasting_modules_constants::reserving_interval));

    if (this->_vertex_data) {
        this->_vertex_data->set_count(rect_count * vertex2d_rect::vector_count);
    }

    if (this->_frame_index_data) {
        this->_frame_index_data->set_count(rect_count * frame_index2d_rect::vector_count);
    }
}
