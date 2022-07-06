//
//  ae_window_receiver.cpp
//

#include "ae_window_receiver.h"

#include <audio_editor_core/ae_action.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_receiver> window_receiver::make_shared() {
    return std::make_shared<window_receiver>();
}

window_receiver::window_receiver() {
}

std::optional<action_id> window_receiver::receivable_id() const {
#warning todo windowのinstance_idを含ませる
    return std::nullopt;
}

std::optional<ae::action> window_receiver::to_action(ae::key const &) const {
    return std::nullopt;
}

void window_receiver::handle_action(ae::action const &) const {
    // do nothing.
}

responding window_receiver::responding_to_action(ae::action const &) const {
    return responding::fallthrough;
}
