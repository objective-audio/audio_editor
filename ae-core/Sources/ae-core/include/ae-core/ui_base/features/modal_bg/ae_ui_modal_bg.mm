//
//  ae_ui_modal_bg.mm
//

#include "ae_ui_modal_bg.h"
#include <ae-core/app/features/ae_color.h>
#include <ae-core/global/ae_ui_hierarchy.h>
#include <ae-core/global/value_types/ae_action.h>
#include <ae-core/project_editing/features/ae_project_action_sender.h>
#include <ae-core/project_editing/lifecycles/ae_project_modal_lifecycle.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_modal_bg> ui_modal_bg::make_shared(project_lifetime_id const &project_lifetime_id,
                                                      std::shared_ptr<ui::node> const &node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);
    return std::make_shared<ui_modal_bg>(resource_lifetime->standard, node, app_lifetime->color.get(),
                                         project_editing_lifetime->modal_lifecycle,
                                         project_editing_lifetime->action_sender);
}

ui_modal_bg::ui_modal_bg(std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::node> const &node,
                         ae::color *color, std::shared_ptr<project_modal_lifecycle> const &project_modal_lifecycle,
                         std::shared_ptr<project_action_sender> const &action_sender)
    : _color(color), _button(ui::button::make_shared({.size = {1.0f, 1.0f}}, standard)), _action_sender(action_sender) {
    auto const &button_node = this->_button->rect_plane()->node();
    node->add_sub_node(button_node);

    this->_button
        ->observe([this](auto const &context) {
            switch (context.phase) {
                case ui::button::phase::ended:
                    if (auto const action_sender = this->_action_sender.lock()) {
                        action_sender->send(time_editing_action_name::cancel_time_editing);
                    }
                    break;
                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    project_modal_lifecycle
        ->observe([this](auto const &sub_lifetime) {
            this->_button->rect_plane()->node()->set_is_enabled(sub_lifetime.has_value());
        })
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
