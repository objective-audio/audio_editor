//
//  project_action_sender.cpp
//

#include "project_action_sender.h"

#include <ae-core/app/features/action_sender.h>
#include <ae-core/global/hierarchy.h>

using namespace yas;
using namespace yas::ae;

project_action_sender::project_action_sender(project_lifetime_id const &project_lifetime_id,
                                             action_sender *action_sender)
    : _project_lifetime_id(project_lifetime_id), _action_sender(action_sender) {
}

void project_action_sender::send(action const &action) {
    this->_action_sender->send(action, action_id{this->_project_lifetime_id});
}

void project_action_sender::send(action_name const &name, std::optional<action_value> &&value) {
    this->send(action{name, std::move(value)});
}

void project_action_sender::send(ae::key const key) {
    action_id const action_id{this->_project_lifetime_id};

    this->_action_sender->send(key, action_id);
}
