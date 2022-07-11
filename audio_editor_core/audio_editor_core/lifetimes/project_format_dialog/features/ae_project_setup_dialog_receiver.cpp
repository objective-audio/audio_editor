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
    return action_id{this->_window_lifetime_id};
}

std::optional<ae::action> project_setup_dialog_receiver::to_action(ae::key const &, ae::action_id const &) const {
    return std::nullopt;
}

void project_setup_dialog_receiver::receive(ae::action const &) const {
    // do nothing.
}

action_receivable_state project_setup_dialog_receiver::receivable_state(ae::action const &) const {
    return action_receivable_state::fallthrough;
}
