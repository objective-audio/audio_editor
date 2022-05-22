//
//  ae_action_controller.cpp
//

#include "ae_action_controller.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_responder_stack.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_controller> action_controller::make_shared(responder_stack *responder_stack) {
    return std::make_shared<action_controller>(responder_stack);
}

action_controller::action_controller(responder_stack *responder_stack) : _responder_stack(responder_stack) {
}

void action_controller::handle_action(action const &action) {
    this->_responder_stack->handle_action(action);
}

void action_controller::handle_key(ae::key const key) {
    if (auto const action = this->_responder_stack->to_action(key)) {
        this->_responder_stack->handle_action(action.value());
    }
}
