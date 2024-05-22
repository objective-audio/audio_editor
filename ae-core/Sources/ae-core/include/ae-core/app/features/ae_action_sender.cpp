//
//  ae_action_sender.cpp
//

#include "ae_action_sender.h"

#include <ae-core/global/interfaces/ae_action_receivable.h>
#include <ae-core/global/interfaces/ae_action_receiver_providable.h>
#include <ae-core/global/value_types/ae_action.h>
#include <ae-core/global/value_types/ae_action_id.h>
#include <cpp-utils/stl_utils.h>

using namespace yas;
using namespace yas::ae;

action_sender::action_sender(action_receiver_providable *root_provider) : _root_provider(root_provider) {
}

void action_sender::send(ae::action const &action, ae::action_id const &action_id) {
    auto const receivers = this->_root_provider->receivers(action_id);

    for (auto const &receiver : receivers) {
        switch (receiver->receivable_state(action)) {
            case ae::action_receivable_state::accepting:
                receiver->receive(action);
                return;
            case ae::action_receivable_state::blocking:
                return;
            case ae::action_receivable_state::fallthrough:
                break;
        }
    }
}

void action_sender::send(ae::key const &key, ae::action_id const &action_id) {
    auto const receivers = this->_root_provider->receivers(action_id);

    for (auto const &receiver : receivers) {
        if (auto const action = receiver->to_action(key)) {
            switch (receiver->receivable_state(action.value())) {
                case ae::action_receivable_state::accepting:
                    receiver->receive(action.value());
                    return;
                case ae::action_receivable_state::blocking:
                    return;
                case ae::action_receivable_state::fallthrough:
                    break;
            }
        }
    }
}

action_receivable_state action_sender::receivable_state(ae::action const &action, ae::action_id const &action_id) {
    auto const receivers = this->_root_provider->receivers(action_id);

    for (auto const &receiver : receivers) {
        switch (receiver->receivable_state(action)) {
            case ae::action_receivable_state::accepting:
                return action_receivable_state::accepting;
            case ae::action_receivable_state::blocking:
                return action_receivable_state::blocking;
            case ae::action_receivable_state::fallthrough:
                break;
        }
    }

    return action_receivable_state::fallthrough;
}
