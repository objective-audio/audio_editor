//
//  ae_time_editor_receiver.cpp
//

#include "ae_time_editor_receiver.h"

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_time_editor_responder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_receiver> time_editor_receiver::make_shared(project_id const &project_id,
                                                                        time_editor_responder *responder) {
    return std::make_shared<time_editor_receiver>(project_id, responder);
}

time_editor_receiver::time_editor_receiver(project_id const &project_id, time_editor_responder *responder)
    : _project_id(project_id), _responder(responder) {
}

std::optional<action_id> time_editor_receiver::receivable_id() const {
    return action_id{.project_id = this->_project_id};
}

std::optional<ae::action> time_editor_receiver::to_action(ae::key const &key) const {
    return this->_responder->to_action(key);
}

void time_editor_receiver::handle_action(ae::action const &action) const {
    return this->_responder->handle_action(action);
}

responding time_editor_receiver::responding_to_action(ae::action const &action) const {
    return this->_responder->responding_to_action(action);
}
