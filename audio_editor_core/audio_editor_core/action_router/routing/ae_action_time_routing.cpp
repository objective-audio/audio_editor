//
//  ae_action_time_routing.cpp
//

#include "ae_action_time_routing.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_time_routing> action_time_routing::make_shared() {
    return std::shared_ptr<action_time_routing>(new action_time_routing{});
}

action_time_routing::action_time_routing() {
}

std::optional<ae::action> action_time_routing::to_action(ae::key const key) {
    switch (key) {
        case ae::key::esc:
            return ae::action::cancel_time_editing;
        case ae::key::num_0:
            return ae::action::input_time_0;
        case ae::key::num_1:
            return ae::action::input_time_1;
        case ae::key::num_2:
            return ae::action::input_time_2;
        case ae::key::num_3:
            return ae::action::input_time_3;
        case ae::key::num_4:
            return ae::action::input_time_4;
        case ae::key::num_5:
            return ae::action::input_time_5;
        case ae::key::num_6:
            return ae::action::input_time_6;
        case ae::key::num_7:
            return ae::action::input_time_7;
        case ae::key::num_8:
            return ae::action::input_time_8;
        case ae::key::num_9:
            return ae::action::input_time_9;

        default:
            return std::nullopt;
    }
}

bool action_time_routing::responds_to_action(ae::action const action) {
    switch (action) {
        case ae::action::cancel_time_editing:
        case ae::action::input_time_0:
        case ae::action::input_time_1:
        case ae::action::input_time_2:
        case ae::action::input_time_3:
        case ae::action::input_time_4:
        case ae::action::input_time_5:
        case ae::action::input_time_6:
        case ae::action::input_time_7:
        case ae::action::input_time_8:
        case ae::action::input_time_9:
            return true;

        default:
            return false;
    }
}
