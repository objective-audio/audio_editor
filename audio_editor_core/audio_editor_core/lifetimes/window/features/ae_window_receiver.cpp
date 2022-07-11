//
//  ae_window_receiver.cpp
//

#include "ae_window_receiver.h"

#include <audio_editor_core/ae_action.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_receiver> window_receiver::make_shared(window_lifetime_id const &lifetime_id) {
    return std::make_shared<window_receiver>(lifetime_id);
}

window_receiver::window_receiver(window_lifetime_id const &lifetime_id) : _lifetime_id(lifetime_id) {
}

std::optional<action_id> window_receiver::receivable_id() const {
    return action_id{this->_lifetime_id};
}

std::optional<ae::action> window_receiver::to_action(ae::key const &, ae::action_id const &) const {
    return std::nullopt;
}

void window_receiver::receive(ae::action const &) const {
    // do nothing.
}

action_receivable_state window_receiver::receivable_state(ae::action const &) const {
    return action_receivable_state::fallthrough;
}
