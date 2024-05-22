//
//  ae_project_receiver.cpp
//

#include "ae_project_receiver.h"

#include <ae-core/global/value_types/ae_action.h>

using namespace yas;
using namespace yas::ae;

project_receiver::project_receiver(project_lifetime_id const &lifetime_id) : _lifetime_id(lifetime_id) {
}

std::optional<action_id> project_receiver::receivable_id() const {
    return action_id{this->_lifetime_id};
}

std::optional<ae::action> project_receiver::to_action(ae::key const &) const {
    return std::nullopt;
}

void project_receiver::receive(ae::action const &) const {
    // do nothing.
}

action_receivable_state project_receiver::receivable_state(ae::action const &) const {
    return action_receivable_state::fallthrough;
}
