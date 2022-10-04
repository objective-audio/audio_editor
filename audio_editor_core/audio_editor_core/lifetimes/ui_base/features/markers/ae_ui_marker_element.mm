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
#include <audio_editor_core/ae_markers_controller.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_marker_element> ui_marker_element::make_shared(window_lifetime_id const &lifetime_id,
                                                                  ui::node *parent_node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    auto const controller = markers_controller::make_shared(lifetime_id);

    return std::shared_ptr<ui_marker_element>(
        new ui_marker_element{project_lifetime->marker_pool, controller, resource_lifetime->standard,
                              app_lifetime->color, resource_lifetime->vertical_line_data,
                              resource_lifetime->square_data, resource_lifetime->normal_font_atlas, parent_node});
}

ui_marker_element::ui_marker_element(std::shared_ptr<marker_pool> const &marker_pool,
                                     std::shared_ptr<markers_controller> const &controller,
                                     std::shared_ptr<ui::standard> const &standard,
                                     std::shared_ptr<ae::color> const &color,
                                     std::shared_ptr<ui_mesh_data> const &vertical_line_data,
                                     std::shared_ptr<ui_mesh_data> const &square_data,
                                     std::shared_ptr<ui::font_atlas> const &font_atlas, ui::node *parent_node)
    : node(ui::node::make_shared()),
      _marker_pool(marker_pool),
      _controller(controller),
      _line_node(ui::node::make_shared()),
      _square_collider_node(ui::node::make_shared()),
      _square_mesh_node(ui::node::make_shared()),
      _strings(ui::strings::make_shared(
          {.frame = ui::region{.origin = ui::point::zero(),
                               .size = {0.0f, -static_cast<float>(font_atlas->ascent() + font_atlas->descent())}}},
          font_atlas)),
      _color(color),
      _collider(ui::collider::make_shared()),
      _touch_tracker(ui::touch_tracker::make_shared(standard, this->_square_collider_node)),
      _multiple_touch(ui::multiple_touch::make_shared()) {
    parent_node->add_sub_node(this->node);

    auto const line_mesh =
        ui::mesh::make_shared({.primitive_type = vertical_line_data->primitive_type}, vertical_line_data->vertex_data,
                              vertical_line_data->index_data, nullptr);
    this->_line_node->set_mesh(line_mesh);

    auto const square_mesh = ui::mesh::make_shared({.primitive_type = square_data->primitive_type},
                                                   square_data->vertex_data, square_data->index_data, nullptr);
    this->_square_mesh_node->set_mesh(square_mesh);

    this->node->add_sub_node(this->_line_node);
    this->node->add_sub_node(this->_square_collider_node);
    this->_square_collider_node->add_sub_node(this->_square_mesh_node);
    this->node->add_sub_node(this->_strings->rect_plane()->node());

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            ui::size const scale{.width = 1.0f, .height = region.size.height};
            this->_line_node->set_scale(scale);
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            this->_line_node->set_color(this->_color->marker_line());
            this->_square_mesh_node->set_color(this->_color->marker_square());
            this->_strings->rect_plane()->node()->set_color(this->_color->marker_text());
        })
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
            this->_square_mesh_node->set_scale(target_region.size);
            this->_collider->set_shape(ui::shape::make_shared({.rect = target_region}));
            this->_square_collider_node->set_colliders({this->_collider});
        })
        .sync()
        ->add_to(this->_pool);

    marker_pool
        ->observe_event([this](marker_pool_event const &event) {
            switch (event.type) {
                case marker_pool_event_type::replaced:
                    if (event.inserted.value().identifier == this->_identifier) {
                        this->_update_name();
                    }
                    break;

                case marker_pool_event_type::any:
                case marker_pool_event_type::reverted:
                case marker_pool_event_type::inserted:
                case marker_pool_event_type::erased:
                    break;
            }
        })
        .end()
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
        ->observe([this](std::uintptr_t const &) {
            if (this->_identifier.has_value()) {
                if (auto const marker_pool = this->_marker_pool.lock()) {
                    if (auto const marker = marker_pool->marker_for_id(this->_identifier.value())) {
                        this->_controller->select_marker_with_index(marker.value().index());
                    }
                }
            }
        })
        .end()
        ->add_to(this->_pool);
}

void ui_marker_element::set_location(marker_location const &location) {
    this->_identifier = location.identifier;
    this->node->set_is_enabled(true);
    this->node->set_position({location.x, this->node->position().y});
    this->_update_name();
}

void ui_marker_element::update_location(marker_location const &location) {
    if (this->_identifier == location.identifier) {
        this->node->set_position({location.x, this->node->position().y});
        this->_update_name();
    } else {
        assertion_failure_if_not_test();
    }
}

void ui_marker_element::reset_location() {
    this->_identifier = std::nullopt;
    this->node->set_is_enabled(false);
}

void ui_marker_element::finalize() {
    this->node->remove_from_super_node();
}

void ui_marker_element::_update_name() {
    if (auto const marker_pool = this->_marker_pool.lock()) {
        if (this->_identifier.has_value()) {
            if (auto const marker = marker_pool->marker_for_id(this->_identifier.value())) {
                this->_strings->set_text(marker->value.name);
            }
        }
    }
}
