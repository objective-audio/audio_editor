//
//  ae_event_handling_presenter.cpp
//

#include "ae_event_handling_presenter.h"

#include <ae-core/app/features/ae_action_sender.h>
#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/global/value_types/ae_action_receivable_state.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<event_handling_presenter> event_handling_presenter::make_shared(
    project_lifetime_id const &project_lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<event_handling_presenter>(project_lifetime_id, app_lifetime->action_sender);
}

event_handling_presenter::event_handling_presenter(project_lifetime_id const &project_lifetime_id,
                                                   std::shared_ptr<action_sender> const &action_sender)
    : _project_lifetime_id(project_lifetime_id), _action_sender(action_sender) {
}

bool event_handling_presenter::responds_to_action(action const action) {
    if (auto const action_sender = this->_action_sender.lock()) {
        switch (action_sender->receivable_state(action, action_id{this->_project_lifetime_id})) {
            case action_receivable_state::accepting:
                return true;

            case action_receivable_state::blocking:
            case action_receivable_state::fallthrough:
                return false;
        }
    } else {
        return false;
    }
}
