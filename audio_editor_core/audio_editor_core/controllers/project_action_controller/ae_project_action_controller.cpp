//
//  ae_project_action_controller.cpp
//

#include "ae_project_action_controller.h"

#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_action_controller> project_action_controller::make_shared(
    window_lifetime_id const &window_lifetime_id, action_sender *action_sender) {
    return std::make_shared<project_action_controller>(window_lifetime_id, action_sender);
}

project_action_controller::project_action_controller(window_lifetime_id const &window_lifetime_id,
                                                     action_sender *action_sender)
    : _window_lifetime_id(window_lifetime_id), _action_sender(action_sender) {
}

void project_action_controller::handle_action(action const &action) {
    this->_action_sender->send(action, action_id{this->_window_lifetime_id});
}

void project_action_controller::handle_action(action_kind const &kind, std::string const &value) {
    this->handle_action(action{kind, value});
}

void project_action_controller::handle_key(ae::key const key) {
    action_id const action_id{this->_window_lifetime_id};

    if (auto const action = this->_action_sender->to_action(key, action_id)) {
        this->_action_sender->send(action.value(), action_id);
    }
}
