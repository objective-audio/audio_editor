//
//  ae_ui_modal_bg.mm
//

#include "ae_ui_modal_bg.h"
#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_action_controller.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_project_sub_level_router.h>
#include <audio_editor_core/ae_ui_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_modal_bg> ui_modal_bg::make_shared(ui_project_id const &ui_project_id,
                                                      std::shared_ptr<ui::standard> const &standard) {
    auto const &app_level = hierarchy::app_level();
    auto const &project_level = hierarchy::project_level_for_id(ui_project_id.project_id);
    return std::make_shared<ui_modal_bg>(standard, app_level->color, project_level->sub_level_router,
                                         project_level->action_controller);
}

ui_modal_bg::ui_modal_bg(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ae::color> const &color,
                         std::shared_ptr<project_sub_level_router> const &project_sub_level_router,
                         std::shared_ptr<action_controller> const &action_controller)
    : node(ui::node::make_shared()),
      _color(color),
      _button(ui::button::make_shared({.size = {1.0f, 1.0f}}, standard)),
      _action_controller(action_controller) {
    auto const &button_node = this->_button->rect_plane()->node();
    this->node->add_sub_node(button_node);

    this->_button
        ->observe([this](auto const &context) {
            switch (context.phase) {
                case ui::button::phase::ended:
                    if (auto const action_controller = this->_action_controller.lock()) {
                        action_controller->handle_action({ae::action_kind::cancel_time_editing});
                    }
                    break;
                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    project_sub_level_router
        ->observe([this](auto const &level) { this->_button->rect_plane()->node()->set_is_enabled(level.has_value()); })
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
