//
//  ae_action_controller.cpp
//

#include "ae_action_controller.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_responder_stack.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_controller> action_controller::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_level->responder_stack);
}

std::shared_ptr<action_controller> action_controller::make_shared(
    std::shared_ptr<responder_stack> const &responder_stack) {
    return std::shared_ptr<action_controller>(new action_controller{responder_stack});
}

action_controller::action_controller(std::shared_ptr<responder_stack> const &responder_stack)
    : _responder_stack(responder_stack), _export_notifier(observing::notifier<url>::make_shared()) {
}

void action_controller::handle_action(action const &action) {
    if (auto const responder_stack = this->_responder_stack.lock()) {
        responder_stack->handle_action(action);
    }
}

void action_controller::handle_key(ae::key const key) {
    if (auto const responder_stack = this->_responder_stack.lock()) {
        if (auto const action = responder_stack->to_action(key)) {
            responder_stack->handle_action(action.value());
        }
    }
}
