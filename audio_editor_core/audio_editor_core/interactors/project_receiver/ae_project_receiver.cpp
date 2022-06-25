//
//  ae_project_receiver.cpp
//

#include "ae_project_receiver.h"

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_project_editor_responder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_receiver> project_receiver::make_shared(project_id const &project_id,
                                                                project_editor_responder *responder) {
    return std::make_shared<project_receiver>(project_id, responder);
}

project_receiver::project_receiver(project_id const &project_id, project_editor_responder *responder)
    : _project_id(project_id), _responder(responder) {
}

std::optional<action_id> project_receiver::receivable_id() const {
    return action_id{.project_id = this->_project_id};
}

std::optional<ae::action> project_receiver::to_action(ae::key const &key) const {
    return this->_responder->to_action(key);
}

void project_receiver::handle_action(ae::action const &action) const {
    this->_responder->handle_action(action);
}

responding project_receiver::responding_to_action(ae::action const &action) const {
    return this->_responder->responding_to_action(action);
}
