//
//  ae_action_sender.cpp
//

#include "ae_action_sender.h"

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_action_id.h>
#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_action_receiver_providable.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_sender> action_sender::make_shared(action_receiver_providable *root_provider) {
    return std::make_shared<action_sender>(root_provider);
}

action_sender::action_sender(action_receiver_providable *root_provider) : _root_provider(root_provider) {
}

std::optional<ae::action> action_sender::to_action(ae::key const &key, ae::action_id const &action_id) const {
    auto const receivers = this->_root_provider->receivers(action_id);

    if (auto const &iterator = receivers.begin(); iterator != receivers.end()) {
        return (*iterator)->to_action(key);
    }

    return std::nullopt;
}

void action_sender::handle_action(ae::action const &action) {
    auto const receivers = this->_root_provider->receivers(action.action_id);

    for (auto const &receiver : receivers) {
        switch (receiver->responding_to_action(action)) {
            case ae::responding::accepting:
                receiver->handle_action(action);
                return;
            case ae::responding::blocking:
                return;
            case ae::responding::fallthrough:
                break;
        }
    }
}

responding action_sender::responding_to_action(ae::action const &action) {
    auto const receivers = this->_root_provider->receivers(action.action_id);

    for (auto const &receiver : receivers) {
        switch (receiver->responding_to_action(action)) {
            case ae::responding::accepting:
                return responding::accepting;
            case ae::responding::blocking:
                return responding::blocking;
            case ae::responding::fallthrough:
                break;
        }
    }

    return responding::fallthrough;
}