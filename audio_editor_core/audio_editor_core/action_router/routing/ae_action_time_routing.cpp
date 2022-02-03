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
        case ae::key::ret:
            return action_kind::finish_time_editing;
        case ae::key::esc:
            return action_kind::cancel_time_editing;
        case ae::key::left:
        case ae::key::shift_tab:
            return action_kind::move_to_previous_time_unit;
        case ae::key::right:
        case ae::key::tab:
            return action_kind::move_to_next_time_unit;
        case ae::key::up:
            return action_kind::increment_time;
        case ae::key::down:
            return action_kind::decrement_time;
        case ae::key::plus:
            return action_kind::change_time_sign_to_plus;
        case ae::key::hyphen:
            return action_kind::change_time_sign_to_minus;

        case ae::key::num_0:
            return action{action_kind::input_time, "0"};
        case ae::key::num_1:
            return action{action_kind::input_time, "1"};
        case ae::key::num_2:
            return action{action_kind::input_time, "2"};
        case ae::key::num_3:
            return action{action_kind::input_time, "3"};
        case ae::key::num_4:
            return action{action_kind::input_time, "4"};
        case ae::key::num_5:
            return action{action_kind::input_time, "5"};
        case ae::key::num_6:
            return action{action_kind::input_time, "6"};
        case ae::key::num_7:
            return action{action_kind::input_time, "7"};
        case ae::key::num_8:
            return action{action_kind::input_time, "8"};
        case ae::key::num_9:
            return action{action_kind::input_time, "9"};
        case ae::key::del:
            return action_kind::delete_time;

        case key::space:
        case key::a:
        case key::s:
        case key::d:
        case key::f:
        case key::n:
        case key::t:
            return std::nullopt;
    }
}

bool action_time_routing::responds_to_action(ae::action_kind const kind) {
    switch (kind) {
        case action_kind::finish_time_editing:
        case action_kind::cancel_time_editing:
        case action_kind::move_to_previous_time_unit:
        case action_kind::move_to_next_time_unit:
        case action_kind::input_time:
        case action_kind::delete_time:
        case action_kind::increment_time:
        case action_kind::decrement_time:
        case action_kind::change_time_sign_to_plus:
        case action_kind::change_time_sign_to_minus:
        case action_kind::select_time_unit:
            return true;

        case action_kind::toggle_play:
        case action_kind::nudge_previous:
        case action_kind::nudge_next:
        case action_kind::rotate_nudging_unit_index:
        case action_kind::rotate_timing_fraction:
        case action_kind::jump_previous:
        case action_kind::jump_next:
        case action_kind::jump_to_beginning:
        case action_kind::jump_to_end:
        case action_kind::drop_head_and_offset:
        case action_kind::split:
        case action_kind::drop_tail_and_offset:
        case action_kind::erase_and_offset:
        case action_kind::insert_marker:
        case action_kind::return_to_zero:
        case action_kind::go_to_marker:
        case action_kind::undo:
        case action_kind::redo:
        case action_kind::select_file_for_export:
        case action_kind::cut:
        case action_kind::copy:
        case action_kind::paste:
        case action_kind::begin_time_editing:
            return false;
    }
}
