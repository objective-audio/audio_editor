//
//  ae_project_setup_dialog_receiver.cpp
//

#include "ae_project_setup_dialog_receiver.h"

#include <audio_editor_core/ae_action.h>

using namespace yas;
using namespace yas::ae;

std::optional<action_id> project_setup_dialog_receiver::receivable_id() const {
#warning todo windowのinstance_idを含ませる
    return std::nullopt;
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
