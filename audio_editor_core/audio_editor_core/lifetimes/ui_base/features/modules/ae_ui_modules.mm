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
#include <audio_editor_core/ae_ui_dynamic_mesh_container.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_modules_constants {
static std::size_t const reserving_interval = 128;
}

namespace yas::ae::ui_modules_utils {
std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> make_fill_container(
    std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>> *vertex_datas) {
    auto make_fill_element = [vertex_datas](std::size_t const idx, std::size_t const element_count) {
        if (vertex_datas->size() == idx) {
            vertex_datas->emplace_back(
                ui::dynamic_mesh_vertex_data::make_shared(element_count * vertex2d_rect::vector_count));
        } else {
            assert(0);
        }

        auto const &vertex_data = vertex_datas->at(idx);
        auto const index_data =
            ui::dynamic_mesh_index_data::make_shared(element_count * fill_index2d_rect::vector_count);
        auto mesh = ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle, .use_mesh_color = true},
                                          vertex_data, index_data, nullptr);

        return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
            .vertex_data = vertex_data, .index_data = std::move(index_data), .mesh = std::move(mesh)});
    };

    return std::make_unique<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>>(
        ui_modules_constants::reserving_interval, std::move(make_fill_element));
}

std::unique_ptr<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>> make_frame_container(
    std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>> *vertex_datas) {
    auto make_frame_element = [vertex_datas](std::size_t const idx, std::size_t const element_count) {
        auto const &vertex_data = vertex_datas->at(idx);
        auto const index_data =
            ui::dynamic_mesh_index_data::make_shared(element_count * frame_index2d_rect::vector_count);
        auto mesh =
            ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, vertex_data, index_data, nullptr);

        return std::unique_ptr<dynamic_mesh_content>(new dynamic_mesh_content{
            .vertex_data = vertex_data, .index_data = std::move(index_data), .mesh = std::move(mesh)});
    };

    return std::make_unique<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>>(
        ui_modules_constants::reserving_interval, std::move(make_frame_element));
}
}

std::shared_ptr<ui_modules> ui_modules::make_shared(project_lifetime_id const &project_lifetime_id,
                                                    std::shared_ptr<ui::node> const &node,
                                                    ui_module_waveforms *waveforms) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    auto const modules_presenter = modules_presenter::make_shared(project_lifetime_id);
    auto const modules_controller = modules_controller::make_shared(project_lifetime_id);
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
      _vertex_datas(std::make_unique<std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>>>()),
      _fill_mesh_container(ui_modules_utils::make_fill_container(this->_vertex_datas.get())),
      _frame_mesh_container(ui_modules_utils::make_frame_container(this->_vertex_datas.get())),
      _names_root_node(ui::node::make_shared()),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_fill_mesh_container->node)),
      _multiple_touch(ui::multiple_touch::make_shared()),
      _modifiers_holder(modifiers_holder) {
    node->add_sub_node(this->_fill_mesh_container->node);
    node->add_sub_node(this->_waveforms->node);
    node->add_sub_node(this->_frame_mesh_container->node);
    node->add_sub_node(this->_names_root_node);

    presenter
        ->observe_contents([this](module_content_pool_event const &event) {
            switch (event.type) {
                case module_content_pool_event_type::fetched:
                case module_content_pool_event_type::replaced:
                    this->_replace_data(event.elements);
                    break;
                case module_content_pool_event_type::updated:
                    this->_update_data(event.elements.size(), event.inserted, event.replaced, event.erased);
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
            auto const &contents = this->_presenter->contents();
            this->_update_colors(contents);
            this->_frame_mesh_container->node->set_color(this->_color->module_frame());
        })
        .sync()
        ->add_to(this->_pool);

    atlas
        ->observe_white_filled_tex_coords(
            [vertex_datas = this->_vertex_datas.get()](ui::uint_region const &tex_coords) {
                for (auto const &vertex_data : *vertex_datas) {
                    vertex_data->write([&tex_coords](std::vector<ui::vertex2d_t> &vertices) {
                        for (auto &vertex : vertices) {
                            vertex.tex_coord = to_float2(tex_coords.origin);
                        }
                    });
                }
            })
        .sync()
        ->add_to(this->_pool);

    this->_touch_tracker
        ->observe([this](ui::touch_tracker::context const &context) {
            if (context.touch_event.touch_id == ui::touch_id::mouse_left()) {
                switch (context.phase) {
                    case ui::touch_tracker_phase::began:
                        this->_began_collider_idx = context.collider_idx;
                        if (this->_modifiers_holder->modifiers().empty()) {
                            this->_controller->begin_range_selection(context.touch_event.position);
                        }
                        break;
                    case ui::touch_tracker_phase::ended:
                        if (this->_began_collider_idx == context.collider_idx) {
                            if (this->_modifiers_holder->modifiers().contains(ae::modifier::command)) {
                                this->_controller->toggle_selection(context.collider_idx);
                            }
                        }
                        this->_began_collider_idx = std::nullopt;
                        break;
                    case ui::touch_tracker_phase::canceled:
                    case ui::touch_tracker_phase::leaved:
                        this->_began_collider_idx = std::nullopt;
                        break;
                    case ui::touch_tracker_phase::moved:
                    case ui::touch_tracker_phase::entered:
                        break;
                }

                this->_multiple_touch->handle_event(context.phase, context.collider_idx);
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_multiple_touch
        ->observe([this](std::uintptr_t const &collider_idx) { this->_controller->begin_renaming(collider_idx); })
        .end()
        ->add_to(this->_pool);

    this->_presenter
        ->observe_range_selection_region([this](std::optional<ui::region> const &region) {
            if (region.has_value()) {
                auto const &region_value = region.value();
                std::vector<std::size_t> hit_indices;

                auto const &colliders = this->_fill_mesh_container->node->colliders();
                auto each = make_fast_each(colliders.size());
                while (yas_each_next(each)) {
                    auto const &idx = yas_each_index(each);
                    auto const &collider = colliders.at(idx);
                    if (collider->hit_test(region_value)) {
                        hit_indices.emplace_back(idx);
                    }
                }

                this->_controller->select(hit_indices);
            }
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_modules::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_fill_mesh_container->node->set_scale(scale);
    this->_frame_mesh_container->node->set_scale(scale);
    this->_waveforms->set_scale(scale);
    this->_update_all_name_positions();
}

void ui_modules::_replace_data(std::vector<std::optional<module_content>> const &contents) {
    this->_set_rect_count(contents.size());

    this->_fill_mesh_container->write_vertex_elements(
        [this, &contents](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const &colliders = this->_fill_mesh_container->node->colliders();
            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            auto const process_length = std::min(range.length, contents.size() - range.index);
            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;

                auto const &content = contents.at(content_idx);
                auto const &collider = colliders.at(content_idx);
                auto &rect = vertex_rects[vertex_idx];

                if (content.has_value()) {
                    auto const &value = content.value();
                    ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                            .size = {.width = value.width(), .height = 1.0f}};
                    rect.set_position(region);
                    rect.set_tex_coord(filled_tex_coords);

                    collider->set_shape(ui::shape::make_shared({.rect = region}));
                    collider->set_enabled(true);
                } else {
                    rect.set_position(ui::region::zero());

                    collider->set_enabled(false);
                    collider->set_shape(nullptr);
                }
            }
        });

    this->_update_colors(contents);

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

// こことは別に_update_dataで部分的に色を更新しているので、変更する際は注意
void ui_modules::_update_colors(std::vector<std::optional<module_content>> const &contents) {
    this->_fill_mesh_container->write_vertex_elements(
        [this, &contents](index_range const range, vertex2d_rect *vertex_rects) {
            if (contents.size() <= range.index) {
                return;
            }

            auto const normal_bg_color = this->_color->module_bg().v;
            auto const selected_bg_color = this->_color->selected_module_bg().v;
            auto const process_length = std::min(range.length, contents.size() - range.index);

            auto each = make_fast_each(process_length);
            while (yas_each_next(each)) {
                auto const &vertex_idx = yas_each_index(each);
                auto const content_idx = range.index + vertex_idx;
                auto const &content = contents.at(content_idx);

                if (content.has_value()) {
                    auto const &bg_color = content.value().is_selected ? selected_bg_color : normal_bg_color;
                    vertex_rects[vertex_idx].set_color(bg_color);
                }
            }
        });

    auto const normal_name_color = this->_color->module_name();
    auto const selected_name_color = this->_color->selected_module_name();

    auto each = make_fast_each(contents.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &content = contents.at(idx);

        if (content.has_value()) {
            auto const &name = this->_names.at(idx);
            auto const &name_color = content.value().is_selected ? selected_name_color : normal_name_color;
            name->rect_plane()->node()->set_color(name_color);
        }
    }
}

void ui_modules::_update_data(std::size_t const count,
                              std::vector<std::pair<std::size_t, module_content>> const &inserted,
                              std::vector<std::pair<std::size_t, module_content>> const &replaced,
                              std::vector<std::pair<std::size_t, module_content>> const &erased) {
    this->_set_rect_count(count);

    this->_fill_mesh_container->write_vertex_elements(
        [&erased, &inserted, &replaced, this](index_range const range, vertex2d_rect *vertex_rects) {
            auto const &colliders = this->_fill_mesh_container->node->colliders();

            for (auto const &pair : erased) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    vertex_rects[vertex_idx].set_position(ui::region::zero());

                    auto const &collider = colliders.at(content_idx);
                    collider->set_enabled(false);
                    collider->set_shape(nullptr);
                }
            }

            auto const &color = this->_color;
            auto const normal_bg_color = color->module_bg().v;
            auto const selected_bg_color = color->selected_module_bg().v;
            auto const &filled_tex_coords = this->_atlas->white_filled_tex_coords();

            for (auto const &pair : inserted) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = pair.second;
                    ui::region const region{.origin = {.x = value.x(), .y = -0.5f},
                                            .size = {.width = value.width(), .height = 1.0f}};

                    auto &rect = vertex_rects[vertex_idx];
                    rect.set_position(region);
                    rect.set_tex_coord(filled_tex_coords);

                    auto const &bg_color = value.is_selected ? selected_bg_color : normal_bg_color;
                    rect.set_color(bg_color);

                    auto const &collider = colliders.at(content_idx);
                    collider->set_shape(ui::shape::make_shared({.rect = region}));
                    collider->set_enabled(true);
                }
            }

            for (auto const &pair : replaced) {
                auto const &content_idx = pair.first;
                if (range.contains(content_idx)) {
                    auto const vertex_idx = content_idx - range.index;
                    auto const &value = pair.second;
                    auto const &bg_color = value.is_selected ? selected_bg_color : normal_bg_color;
                    vertex_rects[vertex_idx].set_color(bg_color);
                }
            }
        });

    auto const normal_name_color = this->_color->module_name();
    auto const selected_name_color = this->_color->selected_module_name();

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
        auto const &name_color = content_value.is_selected ? selected_name_color : normal_name_color;
        this->_names.at(pair.first)->rect_plane()->node()->set_color(name_color);
        strings->set_text(this->_presenter->name_for_index(content_value.index()));
    }

    for (auto const &pair : replaced) {
        auto const &idx = pair.first;
        auto const &content_value = pair.second;
        auto const &strings = this->_names.at(idx);
        this->_update_name_position(idx, content_value);
        auto const &name_color = content_value.is_selected ? selected_name_color : normal_name_color;
        this->_names.at(pair.first)->rect_plane()->node()->set_color(name_color);
        strings->set_text(this->_presenter->name_for_index(content_value.index()));
    }
}

void ui_modules::_set_rect_count(std::size_t const rect_count) {
    this->_fill_mesh_container->set_element_count(rect_count);
    this->_frame_mesh_container->set_element_count(rect_count);

    auto const reserving_count = common_utils::reserving_count(rect_count, ui_modules_constants::reserving_interval);

    if (this->_names.size() < reserving_count) {
        auto each = make_fast_each(reserving_count - this->_names.size());
        while (yas_each_next(each)) {
            auto strings = ui::strings::make_shared({.max_word_count = 32}, this->_name_font_atlas);
            strings->rect_plane()->node()->set_is_enabled(false);
            this->_names_root_node->add_sub_node(strings->rect_plane()->node());
            this->_names.emplace_back(std::move(strings));
        }
    }

    auto const colliders_size = this->_fill_mesh_container->node->colliders().size();
    if (colliders_size < reserving_count) {
        auto each = make_fast_each(reserving_count - colliders_size);
        while (yas_each_next(each)) {
            auto collider = ui::collider::make_shared();
            collider->set_enabled(false);
            this->_fill_mesh_container->node->push_back_collider(std::move(collider));
        }
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
