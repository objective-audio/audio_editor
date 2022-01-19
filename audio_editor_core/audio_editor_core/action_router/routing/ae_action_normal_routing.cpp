//
//  ae_action_normal_routing.cpp
//

#include "ae_action_normal_routing.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<action_normal_routing> action_normal_routing::make_shared() {
    return std::shared_ptr<action_normal_routing>(new action_normal_routing{});
}

action_normal_routing::action_normal_routing() {
}

std::optional<ae::action> action_normal_routing::to_action(ae::key const key) {
    switch (key) {
        case key::space:
            return action::toggle_play;
        case key::del:
            return action::erase_and_offset;
        case key::a:
            return action::drop_head_and_offset;
        case key::s:
            return action::split;
        case key::d:
            return action::drop_tail_and_offset;
        case key::r:
            return action::rotate_nudging_kind;
        case key::f:
            return action::rotate_timing_fraction;
        case key::equal:
            return action::begin_time_editing;
        case key::esc:
            return std::nullopt;
        case key::num_0:
            return action::return_to_zero;
        case key::num_1:
            return action::go_to_marker_1;
        case key::num_2:
            return action::go_to_marker_2;
        case key::num_3:
            return action::go_to_marker_3;
        case key::num_4:
            return action::go_to_marker_4;
        case key::num_5:
            return action::go_to_marker_5;
        case key::num_6:
            return action::go_to_marker_6;
        case key::num_7:
            return action::go_to_marker_7;
        case key::num_8:
            return action::go_to_marker_8;
        case key::num_9:
            return action::go_to_marker_9;
    }
}

bool action_normal_routing::responds_to_action(ae::action const action) {
    switch (action) {
        case ae::action::cancel_time_editing:
            return false;

        default:
            return true;
    }
}
