//
//  ae_event_handling_presenter.cpp
//

#include "ae_event_handling_presenter.h"

#include <audio_editor_core/ae_action_receivable_state.h>
#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<event_handling_presenter> event_handling_presenter::make_shared(
    window_lifetime_id const &window_lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<event_handling_presenter>(window_lifetime_id, app_lifetime->action_sender);
}

event_handling_presenter::event_handling_presenter(window_lifetime_id const &window_lifetime_id,
                                                   std::shared_ptr<action_sender> const &action_sender)
    : _window_lifetime_id(window_lifetime_id), _action_sender(action_sender) {
}

bool event_handling_presenter::responds_to_action(action const action) {
    if (auto const action_sender = this->_action_sender.lock()) {
        switch (action_sender->receivable_state(action, action_id{this->_window_lifetime_id})) {
            case action_receivable_state::accepting:
            case action_receivable_state::blocking:
                return true;

            case action_receivable_state::fallthrough:
                return false;
        }
    } else {
        return false;
    }
}
