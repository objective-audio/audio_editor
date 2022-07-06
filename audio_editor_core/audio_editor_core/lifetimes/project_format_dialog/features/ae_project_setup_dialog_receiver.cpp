//
//  ae_project_setup_dialog_receiver.cpp
//

#include "ae_project_setup_dialog_receiver.h"

#include <audio_editor_core/ae_action.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_setup_dialog_receiver> project_setup_dialog_receiver::make_shared(
    window_lifetime_id const &window_lifetime_id) {
    return std::make_shared<project_setup_dialog_receiver>(window_lifetime_id);
}

project_setup_dialog_receiver::project_setup_dialog_receiver(window_lifetime_id const &window_lifetime_id)
    : _window_lifetime_id(window_lifetime_id) {
}

std::optional<action_id> project_setup_dialog_receiver::receivable_id() const {
    return action_id{.window_instance_id = this->_window_lifetime_id.instance_id,
                     .project_id = this->_window_lifetime_id.project_id};
}

std::optional<ae::action> project_setup_dialog_receiver::to_action(ae::key const &) const {
    return std::nullopt;
}

void project_setup_dialog_receiver::handle_action(ae::action const &) const {
    // do nothing.
}

responding project_setup_dialog_receiver::responding_to_action(ae::action const &) const {
    return responding::fallthrough;
}
