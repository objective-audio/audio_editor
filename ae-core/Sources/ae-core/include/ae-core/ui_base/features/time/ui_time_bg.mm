//
//  ui_time_bg.cpp
//

#include "ui_time_bg.hpp"
#include <ae-core/app/features/color.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/global/ui_hierarchy.h>
#include <ae-core/project_editing/features/project_action_sender.h>
#include <ae-core/ui_base/features/time/ui_time_constants.h>
#include <ae-core/ui_base/utils/ui_button_utils.h>
#include <ae-core/ui_base/features/common/ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_bg> ui_time_bg::make_shared(
    project_lifetime_id const &lifetime_id, std::shared_ptr<ui::node> const &node,
    std::shared_ptr<ui::layout_region_source> const &time_strings_layout) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);

    return std::make_shared<ui_time_bg>(project_editing_lifetime->action_sender, app_lifetime->color.get(),
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

    auto const &bg_mesh = this->_button->rect_plane()->node()->meshes().at(0);
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

    this->_button->set_can_begin_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}, true));
    this->_button->set_can_indicate_tracking(ui_button_utils::is_touch_accepted({ui::touch_id::mouse_left()}, true));

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
