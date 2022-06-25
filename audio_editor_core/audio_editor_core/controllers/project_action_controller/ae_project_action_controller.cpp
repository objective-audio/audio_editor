//
//  ae_project_action_controller.cpp
//

#include "ae_project_action_controller.h"

#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_action_controller> project_action_controller::make_shared(project_id const &project_id,
                                                                                  action_sender *action_sender) {
    return std::make_shared<project_action_controller>(project_id, action_sender);
}

project_action_controller::project_action_controller(project_id const &project_id, action_sender *action_sender)
    : _project_id(project_id), _action_sender(action_sender) {
}

void project_action_controller::handle_action(action const &action) {
    this->_action_sender->handle_action(action);
}

void project_action_controller::handle_action(action_kind const &kind, std::string const &value) {
    this->handle_action(action{kind, action_id{.project_id = this->_project_id}, value});
}

void project_action_controller::handle_key(ae::key const key) {
    if (auto const action = this->_action_sender->to_action(key, action_id{.project_id = this->_project_id})) {
        this->_action_sender->handle_action(action.value());
    }
}
