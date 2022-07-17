//
//  ae_project_action_sender.cpp
//

#include "ae_project_action_sender.h"

#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_action_sender> project_action_sender::make_shared(window_lifetime_id const &window_lifetime_id,
                                                                          action_sender *action_sender) {
    return std::make_shared<project_action_sender>(window_lifetime_id, action_sender);
}

project_action_sender::project_action_sender(window_lifetime_id const &window_lifetime_id, action_sender *action_sender)
    : _window_lifetime_id(window_lifetime_id), _action_sender(action_sender) {
}

void project_action_sender::send(action const &action) {
    this->_action_sender->send(action, action_id{this->_window_lifetime_id});
}

void project_action_sender::send(action_name const &name, std::optional<action_value> &&value) {
    this->send(action{name, std::move(value)});
}

void project_action_sender::send(ae::key const key) {
    action_id const action_id{this->_window_lifetime_id};

    this->_action_sender->send(key, action_id);
}
