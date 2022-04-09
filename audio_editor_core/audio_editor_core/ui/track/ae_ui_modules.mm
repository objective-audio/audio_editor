//
//  ae_ui_modules.mm
//

#include "ae_ui_modules.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_module_waveforms_presenter.h>
#include <audio_editor_core/ae_modules_controller.h>
#include <audio_editor_core/ae_modules_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_modules_constants {
static std::size_t const reserving_interval = 100;
}

std::shared_ptr<ui_modules> ui_modules::make_shared(ui_project_id const &project_id,
                                                    std::shared_ptr<module_location_pool> const &location_pool,
                                                    std::shared_ptr<ui_module_waveforms> const &waveforms) {
    auto const &app_level = app_level::global();
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);

    auto const modules_presenter =
        modules_presenter::make_shared(project_id.identifier, ui_root_level->display_space, location_pool);
    auto const modules_controller = modules_controller::make_shared(project_id.identifier, location_pool);
    return std::shared_ptr<ui_modules>(new ui_modules{modules_presenter, modules_controller, ui_root_level->standard,
                                                      app_level->color, ui_root_level->font_atlas_14, waveforms});
}

ui_modules::ui_modules(std::shared_ptr<modules_presenter> const &presenter,
                       std::shared_ptr<modules_controller> const &controller,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ae::color> const &color,
                       std::shared_ptr<ui::font_atlas> const &name_font_atlas,
                       std::shared_ptr<ui_module_waveforms> const &waveforms)
    : node(ui::node::make_shared()),
      _presenter(presenter),
      _controller(controller),
      _color(color),
      _name_font_atlas(name_font_atlas),
      _waveforms(waveforms),
      _triangle_node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _names_root_node(ui::node::make_shared()),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_triangle_node)),
      _multiple_touch(ui::multiple_touch::make_shared()),
      _triangle_mesh(ui::mesh::make_shared({.use_mesh_color = false}, nullptr, nullptr, nullptr)),
      _line_mesh(ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, nullptr, nullptr, nullptr)) {
    this->node->add_sub_node(this->_triangle_node);
    this->node->add_sub_node(this->_waveforms->node);
    this->node->add_sub_node(this->_line_node);
    this->node->add_sub_node(this->_names_root_node);

    this->node->set_batch(ui::batch::make_shared());

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

            auto const module_name_color = this->_color->module_name();
            for (auto const &name : this->_names) {
                name->rect_plane()->node()->set_color(module_name_color);
            }
        })
        .sync()
        ->add_to(this->_pool);

    this->_touch_tracker
        ->observe([this](ui::touch_tracker::context const &context) {
            if (context.touch_event.touch_id == ui::touch_id::mouse_left()) {
                this->_multiple_touch->handle_event(context.phase, context.collider_idx);
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_multiple_touch
        ->observe([this](std::uintptr_t const &collider_idx) { this->_controller->select_module_at(collider_idx); })
        .end()
        ->add_to(this->_pool);
}

void ui_modules::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_triangle_node->set_scale(scale);
    this->_line_node->set_scale(scale);
    this->_waveforms->set_scale(scale);
    this->_update_all_name_positions();
}

void ui_modules::set_locations(std::vector<std::optional<module_location>> const &locations) {
    this->_set_rect_count(locations.size());

    this->_vertex_data->write(
        [&locations, &colliders = this->_triangle_node->colliders()](std::vector<ui::vertex2d_t> &vertices) {
            auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

            auto each = make_fast_each(locations.size());
            while (yas_each_next(each)) {
                auto const &idx = yas_each_index(each);
                auto const &location = locations.at(idx);
                auto const &collider = colliders.at(idx);

                if (location.has_value()) {
                    auto const &value = location.value();
                    ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                            .size = {.width = value.width(), .height = 1.0f}};
                    vertex_rects[idx].set_position(region);

                    collider->set_shape(ui::shape::make_shared({.rect = region}));
                    collider->set_enabled(true);
                } else {
                    vertex_rects[idx].set_position(ui::region::zero());

                    collider->set_enabled(false);
                    collider->set_shape(nullptr);
                }
            }
        });

    auto each = make_fast_each(locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = locations.at(idx);
        auto const &strings = this->_names.at(idx);
        auto const &node = strings->rect_plane()->node();

        if (location.has_value()) {
            auto const &location_value = location.value();
            node->set_is_enabled(true);
            this->_update_name_position(idx, location.value());
            strings->set_text(location_value.name);
        } else {
            node->set_is_enabled(false);
            strings->set_text("");
        }
    }
}

void ui_modules::update_locations(std::size_t const count,
                                  std::vector<std::pair<std::size_t, module_location>> const &erased,
                                  std::vector<std::pair<std::size_t, module_location>> const &inserted) {
    this->_set_rect_count(count);

    this->_vertex_data->write(
        [&erased, &inserted, &colliders = this->_triangle_node->colliders()](std::vector<ui::vertex2d_t> &vertices) {
            auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

            for (auto const &pair : erased) {
                vertex_rects[pair.first].set_position(ui::region::zero());

                auto const &collider = colliders.at(pair.first);
                collider->set_enabled(false);
                collider->set_shape(nullptr);
            }

            for (auto const &pair : inserted) {
                auto const &value = pair.second;
                ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                        .size = {.width = value.width(), .height = 1.0f}};

                vertex_rects[pair.first].set_position(region);

                auto const &collider = colliders.at(pair.first);
                collider->set_shape(ui::shape::make_shared({.rect = region}));
                collider->set_enabled(true);
            }
        });

    for (auto const &pair : erased) {
        auto const &idx = pair.first;
        auto const &strings = this->_names.at(idx);
        auto const &node = strings->rect_plane()->node();
        node->set_is_enabled(false);
    }

    for (auto const &pair : inserted) {
        auto const &idx = pair.first;
        auto const &location_value = pair.second;
        auto const &strings = this->_names.at(idx);
        auto const &node = strings->rect_plane()->node();
        node->set_is_enabled(true);
        this->_update_name_position(idx, location_value);
        strings->set_text(location_value.name);
    }
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
    this->_triangle_node->set_colliders({});
    this->_names_root_node->remove_all_sub_nodes();

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

    if (this->_names.size() < max_count) {
        auto const module_name_color = this->_color->module_name();

        auto each = make_fast_each(max_count - this->_names.size());
        while (yas_each_next(each)) {
            auto strings = ui::strings::make_shared({.max_word_count = 32}, this->_name_font_atlas);
            strings->rect_plane()->node()->set_is_enabled(false);
            strings->rect_plane()->node()->set_color(module_name_color);
            this->_names.emplace_back(std::move(strings));
        }
    }

    {
        std::vector<std::shared_ptr<ui::collider>> colliders;
        colliders.reserve(max_count);

        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto collider = ui::collider::make_shared();
            collider->set_enabled(false);
            colliders.emplace_back(std::move(collider));
        }

        this->_triangle_node->set_colliders(std::move(colliders));
    }

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

    auto const &names_root_sub_nodes = this->_names_root_node->sub_nodes();

    auto each = make_fast_each(names_root_sub_nodes.size(), rect_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &name = this->_names.at(idx);
        auto const &name_node = name->rect_plane()->node();
        this->_names_root_node->add_sub_node(name_node);
    }
}

void ui_modules::_update_all_name_positions() {
    auto const &locations = this->_presenter->locations();

    auto each = make_fast_each(locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = locations.at(idx);

        if (location.has_value()) {
            this->_update_name_position(idx, location.value());
        }
    }
}

void ui_modules::_update_name_position(std::size_t const idx, ae::module_location const &location_value) {
    auto const &strings = this->_names.at(idx);
    auto const &node = strings->rect_plane()->node();

    node->set_position({.x = location_value.x() * this->_scale.width, .y = this->_scale.height * 0.5f});
    strings->preferred_layout_guide()->set_region(
        {.origin = {.x = 0.0f, .y = -this->_scale.height},
         .size = {.width = location_value.width() * this->_scale.width, .height = this->_scale.height}});
}
