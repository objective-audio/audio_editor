//
//  ae_ui_modules.mm
//

#include "ae_ui_modules.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_module_content.h>
#include <audio_editor_core/ae_module_waveforms_presenter.h>
#include <audio_editor_core/ae_modules_controller.h>
#include <audio_editor_core/ae_modules_presenter.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_modifiers_holder.hpp>
#include <audio_editor_core/ae_ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_modules_constants {
static std::size_t const reserving_interval = 100;
}

std::shared_ptr<ui_modules> ui_modules::make_shared(window_lifetime_id const &window_lifetime_id,
                                                    std::shared_ptr<ui::node> const &node,
                                                    ui_module_waveforms *waveforms) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    auto const modules_presenter = modules_presenter::make_shared(window_lifetime_id, resource_lifetime->display_space);
    auto const modules_controller = modules_controller::make_shared(window_lifetime_id);
    return std::make_shared<ui_modules>(modules_presenter, modules_controller, resource_lifetime->standard, node,
                                        app_lifetime->color.get(), resource_lifetime->normal_font_atlas, waveforms,
                                        resource_lifetime->modifiers_holder.get(), resource_lifetime->atlas.get());
}

ui_modules::ui_modules(std::shared_ptr<modules_presenter> const &presenter,
                       std::shared_ptr<modules_controller> const &controller,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::node> const &node,
                       ae::color *color, std::shared_ptr<ui::font_atlas> const &name_font_atlas,
                       ui_module_waveforms *waveforms, modifiers_holder *modifiers_holder, ui_atlas const *atlas)
    : _presenter(presenter),
      _controller(controller),
      _color(color),
      _name_font_atlas(name_font_atlas),
      _atlas(atlas),
      _waveforms(waveforms),
      _triangle_node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _names_root_node(ui::node::make_shared()),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_triangle_node)),
      _multiple_touch(ui::multiple_touch::make_shared()),
      _modifiers_holder(modifiers_holder),
      _triangle_mesh(ui::mesh::make_shared({.use_mesh_color = false}, nullptr, nullptr, atlas->texture())),
      _line_mesh(ui::mesh::make_shared({.primitive_type = ui::primitive_type::line, .use_mesh_color = true}, nullptr,
                                       nullptr, atlas->texture())) {
    node->add_sub_node(this->_triangle_node);
    node->add_sub_node(this->_waveforms->node);
    node->add_sub_node(this->_line_node);
    node->add_sub_node(this->_names_root_node);

    this->_triangle_node->set_mesh(this->_triangle_mesh);
    this->_line_node->set_mesh(this->_line_mesh);

    this->_set_rect_count(0);

    presenter
        ->observe_contents([this](module_content_pool_event const &event) {
            switch (event.type) {
                case module_content_pool_event_type::fetched:
                case module_content_pool_event_type::replaced:
                    this->_set_contents(event.elements);
                    break;
                case module_content_pool_event_type::updated:
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
        ->observe_appearance([this](auto const &) {
            this->_vertex_data->write([this](std::vector<ui::vertex2d_t> &vertices) {
                auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();
                auto const &contents = this->_presenter->contents();
                auto const normal_frame_color = this->_color->module_frame().v;
                auto const selected_frame_color = this->_color->selected_module_frame().v;

                auto each = make_fast_each(contents.size());
                while (yas_each_next(each)) {
                    auto const &idx = yas_each_index(each);
                    auto const &content = contents.at(idx);

                    if (content.has_value()) {
                        auto const &frame_color =
                            content.value().is_selected ? selected_frame_color : normal_frame_color;
                        vertex_rects[idx].set_color(frame_color);
                    }
                }
            });

            this->_triangle_node->set_color(this->_color->module_bg());

            auto const module_name_color = this->_color->module_name();
            for (auto const &name : this->_names) {
                name->rect_plane()->node()->set_color(module_name_color);
            }
        })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords([this](ui::uint_region const &tex_coords) {
            this->_vertex_data->write([&tex_coords](std::vector<ui::vertex2d_t> &vertices) {
                for (auto &vertex : vertices) {
                    vertex.tex_coord = to_float2(tex_coords.origin);
                }
            });
        })
        .sync()
        ->add_to(this->_pool);

    this->_touch_tracker
        ->observe([this](ui::touch_tracker::context const &context) {
            if (context.touch_event.touch_id == ui::touch_id::mouse_left()) {
                if (context.phase == ui::touch_tracker_phase::ended) {
                    if (this->_modifiers_holder->modifiers().contains(ae::modifier::command)) {
                        this->_controller->toggle_module_selection_at(context.collider_idx);
                    } else {
                        this->_controller->select_module_at(context.collider_idx);
                    }
                }
                this->_multiple_touch->handle_event(context.phase, context.collider_idx);
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_multiple_touch
        ->observe(
            [this](std::uintptr_t const &collider_idx) { this->_controller->begin_module_renaming_at(collider_idx); })
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

void ui_modules::_set_contents(std::vector<std::optional<module_content>> const &contents) {
    this->_set_rect_count(contents.size());

    this->_vertex_data->write([&contents, this](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

        auto const normal_frame_color = this->_color->module_frame().v;
        auto const selected_frame_color = this->_color->selected_module_frame().v;
        auto const &colliders = this->_triangle_node->colliders();
        auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

        auto each = make_fast_each(contents.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &content = contents.at(idx);
            auto const &collider = colliders.at(idx);

            if (content.has_value()) {
                auto const &value = content.value();
                ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                        .size = {.width = value.width(), .height = 1.0f}};
                auto &rect = vertex_rects[idx];
                rect.set_position(region);
                rect.set_tex_coord(filled_tex_coords);

                auto const &frame_color = value.is_selected ? selected_frame_color : normal_frame_color;
                rect.set_color(frame_color);

                collider->set_shape(ui::shape::make_shared({.rect = region}));
                collider->set_enabled(true);
            } else {
                vertex_rects[idx].set_position(ui::region::zero());

                collider->set_enabled(false);
                collider->set_shape(nullptr);
            }
        }
    });

    auto each = make_fast_each(contents.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &content = contents.at(idx);
        auto const &strings = this->_names.at(idx);
        auto const &node = strings->rect_plane()->node();

        if (content.has_value()) {
            auto const &content_value = content.value();
            node->set_is_enabled(true);
            this->_update_name_position(idx, content.value());
            strings->set_text(this->_presenter->name_for_index(content_value.index()));
        } else {
            node->set_is_enabled(false);
            strings->set_text("");
        }
    }
}

void ui_modules::_update_contents(std::size_t const count,
                                  std::vector<std::pair<std::size_t, module_content>> const &inserted,
                                  std::vector<std::pair<std::size_t, module_content>> const &replaced,
                                  std::vector<std::pair<std::size_t, module_content>> const &erased) {
    this->_set_rect_count(count);

    this->_vertex_data->write([&erased, &inserted, &replaced, &colliders = this->_triangle_node->colliders(),
                               &color = this->_color](std::vector<ui::vertex2d_t> &vertices) {
        auto *vertex_rects = (ui::vertex2d_rect *)vertices.data();

        for (auto const &pair : erased) {
            vertex_rects[pair.first].set_position(ui::region::zero());

            auto const &collider = colliders.at(pair.first);
            collider->set_enabled(false);
            collider->set_shape(nullptr);
        }

        auto const normal_frame_color = color->module_frame().v;
        auto const selected_frame_color = color->selected_module_frame().v;

        for (auto const &pair : inserted) {
            auto const &value = pair.second;
            ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                    .size = {.width = value.width(), .height = 1.0f}};

            auto &rect = vertex_rects[pair.first];
            rect.set_position(region);

            auto const &frame_color = value.is_selected ? selected_frame_color : normal_frame_color;
            rect.set_color(frame_color);

            auto const &collider = colliders.at(pair.first);
            collider->set_shape(ui::shape::make_shared({.rect = region}));
            collider->set_enabled(true);
        }

        for (auto const &pair : replaced) {
            auto const &value = pair.second;
            auto const &frame_color = value.is_selected ? selected_frame_color : normal_frame_color;
            vertex_rects[pair.first].set_color(frame_color);
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
        auto const &content_value = pair.second;
        auto const &strings = this->_names.at(idx);
        auto const &node = strings->rect_plane()->node();
        node->set_is_enabled(true);
        this->_update_name_position(idx, content_value);
        strings->set_text(this->_presenter->name_for_index(content_value.index()));
    }

    for (auto const &pair : replaced) {
        auto const &idx = pair.first;
        auto const &content_value = pair.second;
        auto const &strings = this->_names.at(idx);
        this->_update_name_position(idx, content_value);
        strings->set_text(this->_presenter->name_for_index(content_value.index()));
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
    auto const &contents = this->_presenter->contents();

    auto each = make_fast_each(contents.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &content = contents.at(idx);

        if (content.has_value()) {
            this->_update_name_position(idx, content.value());
        }
    }
}

void ui_modules::_update_name_position(std::size_t const idx, ae::module_content const &content) {
    auto const &strings = this->_names.at(idx);
    auto const &node = strings->rect_plane()->node();

    node->set_position({.x = content.x() * this->_scale.width, .y = this->_scale.height * 0.5f});
    strings->preferred_layout_guide()->set_region(
        {.origin = {.x = 0.0f, .y = -this->_scale.height},
         .size = {.width = content.width() * this->_scale.width, .height = this->_scale.height}});
}
