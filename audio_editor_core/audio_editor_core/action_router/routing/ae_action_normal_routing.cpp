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
        case key::left:
            return action::nudge_previous;
        case key::right:
            return action::nudge_next;
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

        case key::esc:
        case key::plus:
        case key::hyphen:
        case key::ret:
        case key::tab:
        case key::shift_tab:
            return std::nullopt;
    }
}

bool action_normal_routing::responds_to_action(ae::action const action) {
    switch (action) {
        case action::toggle_play:
        case action::nudge_previous:
        case action::nudge_next:
        case action::rotate_nudging_kind:
        case action::rotate_timing_fraction:
        case action::jump_previous:
        case action::jump_next:
        case action::jump_to_beginning:
        case action::jump_to_end:
        case action::drop_head_and_offset:
        case action::split:
        case action::drop_tail_and_offset:
        case action::erase_and_offset:
        case action::insert_marker:
        case action::return_to_zero:
        case action::go_to_marker_1:
        case action::go_to_marker_2:
        case action::go_to_marker_3:
        case action::go_to_marker_4:
        case action::go_to_marker_5:
        case action::go_to_marker_6:
        case action::go_to_marker_7:
        case action::go_to_marker_8:
        case action::go_to_marker_9:
        case action::undo:
        case action::redo:
        case action::select_file_for_export:
        case action::cut:
        case action::copy:
        case action::paste:
        case action::begin_time_editing:
            return true;

        case action::finish_time_editing:
        case action::cancel_time_editing:
        case action::move_to_next_time_unit:
        case action::move_to_previous_time_unit:
        case action::input_time_0:
        case action::input_time_1:
        case action::input_time_2:
        case action::input_time_3:
        case action::input_time_4:
        case action::input_time_5:
        case action::input_time_6:
        case action::input_time_7:
        case action::input_time_8:
        case action::input_time_9:
        case action::delete_time:
        case action::change_time_sign_to_plus:
        case action::change_time_sign_to_minus:
            return false;
    }
}
