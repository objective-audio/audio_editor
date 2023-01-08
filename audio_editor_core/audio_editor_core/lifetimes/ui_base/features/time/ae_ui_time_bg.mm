//
//  ae_ui_time_bg.cpp
//

#include "ae_ui_time_bg.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_ui_button_utils.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_time_constants.h>
#include <audio_editor_core/ae_ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_bg> ui_time_bg::make_shared(
    window_lifetime_id const &lifetime_id, std::shared_ptr<ui::node> const &node,
    std::shared_ptr<ui::layout_region_source> const &time_strings_layout) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);

    return std::make_shared<ui_time_bg>(project_lifetime->action_sender, app_lifetime->color.get(),
                                        resource_lifetime->standard, node, time_strings_layout,
                                        resource_lifetime->atlas.get());
}

ui_time_bg::ui_time_bg(std::shared_ptr<project_action_sender> const &action_sender, ae::color *color,
                       std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::node> const &node,
                       std::shared_ptr<ui::layout_region_source> const &time_strings_layout, ui_atlas const *atlas)
    : _action_sender(action_sender),
      _color(color),
      _button(ui::button::make_shared(ui::region{.size = {1.0f, 1.0f}}, standard)) {
    node->add_sub_node(this->_button->rect_plane()->node());

    auto const &bg_mesh = this->_button->rect_plane()->node()->mesh();
    bg_mesh->set_use_mesh_color(true);
    bg_mesh->set_texture(atlas->texture());

    atlas
        ->observe_white_filled_tex_coords([this](ui::uint_region const &tex_coords) {
            auto const &bg_data = this->_button->rect_plane()->data();
            bg_data->set_rect_tex_coords(tex_coords, 0);
            bg_data->set_rect_tex_coords(tex_coords, 1);
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const &bg_data = this->_button->rect_plane()->data();
            bg_data->set_rect_color(this->_color->time_bg(), to_rect_index(0, false));
            bg_data->set_rect_color(this->_color->time_bg_tracking(), to_rect_index(0, true));
        })
        .sync()
        ->add_to(this->_pool);

    this->_button->set_can_begin_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));
    this->_button->set_can_indicate_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}));

    this->_button
        ->observe([this](ui::button::context const &context) {
            switch (context.phase) {
                case ui::button::phase::ended:
                    if (context.touch.touch_id == ui::touch_id::mouse_left()) {
                        if (auto const action_sender = this->_action_sender.lock()) {
                            action_sender->send(editing_action_name::begin_time_editing);
                        }
                    }
                    break;
                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    time_strings_layout
        ->observe_layout_region([this](ui::region const &region) {
            auto const bg_button_node = this->_button->rect_plane()->node();
            float const nudge_height = ui_time_constants::nudge_height;
            bg_button_node->set_position(region.origin - ui::point{.x = 0.0f, .y = nudge_height});
            bg_button_node->set_scale({.width = region.size.width, .height = region.size.height + nudge_height});
        })
        .sync()
        ->add_to(this->_pool);
}
