//
//  ae_ui_marker_element.cpp
//

#include "ae_ui_marker_element.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_marker_constants.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <cpp_utils/yas_assertion.h>
#include <audio_editor_core/ae_marker_element_controller.hpp>
#include <audio_editor_core/ae_modifiers_holder.hpp>
#include <audio_editor_core/ae_ui_atlas.hpp>
#include <audio_editor_core/ae_ui_square_mesh_data.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_marker_element> ui_marker_element::make_shared(project_lifetime_id const &lifetime_id,
                                                                  ui::node *parent_node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    auto const controller = marker_element_controller::make_shared(lifetime_id);

    return std::shared_ptr<ui_marker_element>(new ui_marker_element{
        project_editing_lifetime->marker_pool, project_editing_lifetime->selected_marker_pool, controller,
        resource_lifetime->standard, app_lifetime->color, resource_lifetime->square_mesh_data,
        resource_lifetime->normal_font_atlas, parent_node, resource_lifetime->modifiers_holder.get()});
}

ui_marker_element::ui_marker_element(
    std::shared_ptr<marker_pool> const &marker_pool, std::shared_ptr<selected_marker_pool> const &selected_marker_pool,
    std::shared_ptr<marker_element_controller> const &controller, std::shared_ptr<ui::standard> const &standard,
    std::shared_ptr<ae::color> const &color, std::shared_ptr<ui_square_mesh_data> const &square_data,
    std::shared_ptr<ui::font_atlas> const &font_atlas, ui::node *parent_node, modifiers_holder *modifiers_holder)
    : _node(ui::node::make_shared()),
      _marker_pool(marker_pool),
      _selected_marker_pool(selected_marker_pool),
      _controller(controller),
      _line_node(ui::node::make_shared()),
      _square_collider_node(ui::node::make_shared()),
      _square_node(ui::node::make_shared()),
      _strings(ui::strings::make_shared(
          {.frame = ui::region{.origin = ui::point::zero(),
                               .size = {0.0f, -static_cast<float>(font_atlas->ascent() + font_atlas->descent())}}},
          font_atlas)),
      _color(color),
      _collider(ui::collider::make_shared()),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_square_collider_node)),
      _multiple_touch(ui::multiple_touch::make_shared()),
      _modifiers_holder(modifiers_holder) {
    parent_node->add_sub_node(this->_node);

    auto const line_mesh =
        ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle}, square_data->vertex_data(),
                              square_data->index_data(), square_data->texture());
    this->_line_node->set_meshes({line_mesh});

    auto const square_mesh =
        ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle}, square_data->vertex_data(),
                              square_data->index_data(), square_data->texture());
    this->_square_node->set_meshes({square_mesh});

    this->_node->add_sub_node(this->_line_node);
    this->_node->add_sub_node(this->_square_collider_node);
    this->_square_collider_node->add_sub_node(this->_square_node);
    this->_node->add_sub_node(this->_strings->rect_plane()->node());

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            this->_line_node->set_position({.x = 0.0f, .y = -region.size.height * 0.5f});
            this->_line_node->set_scale({.width = 0.5f, .height = region.size.height});
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_color(); })
        .sync()
        ->add_to(this->_pool);

    auto const &view_top_guide = standard->view_look()->view_layout_guide()->top();

    this->_strings->rect_plane()->node()->attach_y_layout_guide(*view_top_guide);
    this->_square_collider_node->attach_y_layout_guide(*view_top_guide);

    float const font_height = font_atlas->ascent() + font_atlas->descent();
    this->_strings->actual_layout_source()
        ->observe_layout_region([this, font_height](ui::region const &region) {
            ui::region const target_region{
                .size = {.width = std::max(region.size.width, ui_marker_constants::min_square_width),
                         .height = -font_height}};
            this->_square_node->set_scale(target_region.size);
            this->_collider->set_shape(ui::shape::make_shared({.rect = target_region}));
            this->_square_collider_node->set_colliders({this->_collider});
        })
        .sync()
        ->add_to(this->_pool);

    this->_touch_tracker
        ->observe([this](ui::touch_tracker::context const &context) {
            if (context.touch_event.touch_id == ui::touch_id::mouse_left()) {
                auto const &modifiers = this->_modifiers_holder->modifiers();
                if (context.phase == ui::touch_tracker_phase::began) {
                    if (auto const marker_index = this->marker_index()) {
                        if (modifiers.empty()) {
                            this->_controller->deselect_all();
                            this->_controller->begin_range_selection(context.touch_event.position);
                        } else if (modifiers.size() == 1 && modifiers.contains(ae::modifier::shift)) {
                            this->_controller->begin_range_selection(context.touch_event.position);
                        }
                    }
                } else if (context.phase == ui::touch_tracker_phase::ended) {
                    if (auto const marker_index = this->marker_index()) {
                        if (modifiers.contains(ae::modifier::command)) {
                            this->_controller->toggle_selection(marker_index.value());
                        }
                    }
                }
                this->_multiple_touch->handle_event(context.phase, context.collider_idx);
            }
        })
        .end()
        ->add_to(this->_pool);

    this->_multiple_touch
        ->observe([this](std::uintptr_t const &) {
            if (auto const marker_index = this->marker_index()) {
                this->_controller->begin_renaming(marker_index.value());
            }
        })
        .end()
        ->add_to(this->_pool);
}

void ui_marker_element::set_content(marker_content const &content) {
    this->_content = content;
    this->_node->set_is_enabled(true);
    this->_node->set_x(content.x);
    this->_update_name();
    this->_update_color();
}

void ui_marker_element::update_content(marker_content const &content) {
    if (this->_content.has_value() && this->_content.value().identifier == content.identifier) {
        this->_content = content;
        this->_node->set_x(content.x);
        this->_update_name();
        this->_update_color();
    } else {
        assertion_failure_if_not_test();
    }
}

void ui_marker_element::reset_content() {
    this->_content = std::nullopt;
    this->_node->set_is_enabled(false);
}

std::optional<marker_index> ui_marker_element::marker_index() const {
    if (this->_content.has_value()) {
        if (auto const marker_pool = this->_marker_pool.lock()) {
            if (auto const marker = marker_pool->marker_for_id(this->_content.value().identifier)) {
                return marker.value().index();
            }
        }
    }
    return std::nullopt;
}

bool ui_marker_element::hit_test(ui::region const &rect) {
    if (this->_content.has_value()) {
        return this->_collider->hit_test(rect);
    } else {
        return false;
    }
}

void ui_marker_element::finalize() {
    this->_node->remove_from_super_node();
}

void ui_marker_element::_update_name() {
    if (auto const marker_pool = this->_marker_pool.lock()) {
        if (this->_content.has_value()) {
            this->_strings->set_text(this->_content.value().name);
        }
    }
}

void ui_marker_element::_update_color() {
    if (auto const &content = this->_content) {
        bool const is_selected = content.value().is_selected;
        auto const square_color = is_selected ? this->_color->selected_marker_square() : this->_color->marker_square();
        auto const text_color = is_selected ? this->_color->selected_marker_text() : this->_color->marker_text();

        this->_line_node->set_color(this->_color->marker_line());
        this->_square_node->set_color(square_color);
        this->_strings->rect_plane()->node()->set_color(text_color);
    }
}
