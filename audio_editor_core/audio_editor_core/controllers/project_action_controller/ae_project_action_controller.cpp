//
//  ae_project_action_controller.cpp
//

#include "ae_project_action_controller.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_responder_stack.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_action_controller> project_action_controller::make_shared(responder_stack *responder_stack) {
    return std::make_shared<project_action_controller>(responder_stack);
}

project_action_controller::project_action_controller(responder_stack *responder_stack)
    : _responder_stack(responder_stack) {
}

void project_action_controller::handle_action(action const &action) {
    this->_responder_stack->handle_action(action);
}

void project_action_controller::handle_key(ae::key const key) {
    if (auto const action = this->_responder_stack->to_action(key)) {
        this->_responder_stack->handle_action(action.value());
    }
}