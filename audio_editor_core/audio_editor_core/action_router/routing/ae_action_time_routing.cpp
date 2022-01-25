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
            return action::finish_time_editing;
        case ae::key::esc:
            return action::cancel_time_editing;
        case ae::key::left:
        case ae::key::shift_tab:
            return action::move_to_previous_time_unit;
        case ae::key::right:
        case ae::key::tab:
            return action::move_to_next_time_unit;
        case ae::key::plus:
            return action::change_time_sign_to_plus;
        case ae::key::hyphen:
            return action::change_time_sign_to_minus;

        case ae::key::num_0:
            return action::input_time_0;
        case ae::key::num_1:
            return action::input_time_1;
        case ae::key::num_2:
            return action::input_time_2;
        case ae::key::num_3:
            return action::input_time_3;
        case ae::key::num_4:
            return action::input_time_4;
        case ae::key::num_5:
            return action::input_time_5;
        case ae::key::num_6:
            return action::input_time_6;
        case ae::key::num_7:
            return action::input_time_7;
        case ae::key::num_8:
            return action::input_time_8;
        case ae::key::num_9:
            return action::input_time_9;
        case ae::key::del:
            return action::delete_time;

        case key::space:
        case key::a:
        case key::s:
        case key::d:
        case key::f:
        case key::r:
        case key::equal:
            return std::nullopt;
    }
}

bool action_time_routing::responds_to_action(ae::action const action) {
    switch (action) {
        case action::finish_time_editing:
        case action::cancel_time_editing:
        case action::move_to_previous_time_unit:
        case action::move_to_next_time_unit:
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
            return true;

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
            return false;
    }
}
