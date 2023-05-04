//
//  ae_ui_event_handling.cpp
//

#include "ae_ui_event_handling.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_event_handling_presenter.h>
#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_modifiers_holder.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_event_handling> ui_event_handling::make_shared(project_lifetime_id const &project_lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(project_lifetime_id);

    return std::make_shared<ui_event_handling>(resource_lifetime->standard, project_editing_lifetime->action_sender,
                                               project_editing_lifetime->pinch_gesture_controller,
                                               resource_lifetime->keyboard, resource_lifetime->modifiers_holder);
}

ui_event_handling::ui_event_handling(std::shared_ptr<ui::standard> const &standard,
                                     std::shared_ptr<project_action_sender> const &action_sender,
                                     std::shared_ptr<pinch_gesture_controller> const &pinch_gesture_controller,
                                     std::shared_ptr<ae::keyboard> const &keyboard,
                                     std::shared_ptr<ae::modifiers_holder> const &modifiers_holder)
    : _action_sender(action_sender), _pinch_gesture_controller(pinch_gesture_controller) {
    keyboard
        ->observe([this](ae::key const &key) {
            if (auto const action_sender = this->_action_sender.lock()) {
                action_sender->send(key);
            }
        })
        .end()
        ->add_to(this->_pool);

    modifiers_holder
        ->observe([this](ae::modifiers_holder_event const &event) {
            switch (event.modifier) {
                case ae::modifier::shift:
                    if (auto const controller = this->_pinch_gesture_controller.lock()) {
                        controller->handle_modifier(event.state);
                    }
                    break;

                default:
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);

    standard->event_manager()
        ->observe([this](std::shared_ptr<ui::event> const &event) {
            if (event->type() == ui::event_type::pinch) {
                auto const &pinch_event = event->get<ui::pinch>();
                gesture_state const state = to_gesture_state(event->phase());

                if (auto const controller = this->_pinch_gesture_controller.lock()) {
                    controller->handle_gesture(
                        pinch_gesture{.state = state, .magnification = pinch_event.magnification});
                }
            }
        })
        .end()
        ->add_to(this->_pool);
}
