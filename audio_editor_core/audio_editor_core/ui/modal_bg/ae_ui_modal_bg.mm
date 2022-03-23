//
//  ae_ui_modal_bg.mm
//

#include "ae_ui_modal_bg.h"
#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_time_editor_level_pool.h>
#include <audio_editor_core/ae_ui_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_modal_bg> ui_modal_bg::make_shared(ui_project_id const &project_id) {
    auto const &app_level = hierarchy::app_level();
    auto const &project_editor_level = hierarchy::project_editor_level_for_id(project_id.identifier);
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);
    auto const action_controller = action_controller::make_shared(project_id.identifier);
    return std::shared_ptr<ui_modal_bg>(new ui_modal_bg{
        ui_root_level->standard, app_level->color, project_editor_level->time_editor_level_pool, action_controller});
}

ui_modal_bg::ui_modal_bg(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ae::color> const &color,
                         std::shared_ptr<time_editor_level_pool> const &time_editor_level_pool,
                         std::shared_ptr<action_controller> const &action_controller)
    : node(ui::node::make_shared()),
      _color(color),
      _button(ui::button::make_shared({.size = {1.0f, 1.0f}}, standard->event_manager(), standard->detector())),
      _action_controller(action_controller) {
    auto const &button_node = this->_button->rect_plane()->node();
    this->node->add_sub_node(button_node);

    this->_button
        ->observe([this](auto const &context) {
            switch (context.method) {
                case ui::button::method::ended:
                    this->_action_controller->handle_action({ae::action_kind::cancel_time_editing});
                    break;
                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    time_editor_level_pool
        ->observe_level(
            [this](auto const &level) { this->_button->rect_plane()->node()->set_is_enabled(level != nullptr); })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            auto const &button_node = this->_button->rect_plane()->node();
            button_node->set_position(region.origin);
            button_node->set_scale(region.size);
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance(
            [this](auto const &) { this->_button->rect_plane()->node()->set_color(this->_color->modal_bg()); })
        .sync()
        ->add_to(this->_pool);
}
