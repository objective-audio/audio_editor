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
            return action_kind::toggle_play;
        case key::del:
            return action_kind::erase_and_offset;
        case key::a:
            return action_kind::drop_head_and_offset;
        case key::s:
            return action_kind::split;
        case key::d:
            return action_kind::drop_tail_and_offset;
        case key::n:
            return action_kind::rotate_nudging_unit_index;
        case key::f:
            return action_kind::rotate_timing_fraction;
        case key::t:
            return action_kind::begin_time_editing;
        case key::left:
            return action_kind::nudge_previous;
        case key::right:
            return action_kind::nudge_next;
        case key::num_0:
            return action_kind::return_to_zero;
        case key::num_1:
            return action{action_kind::go_to_marker, "0"};
        case key::num_2:
            return action{action_kind::go_to_marker, "1"};
        case key::num_3:
            return action{action_kind::go_to_marker, "2"};
        case key::num_4:
            return action{action_kind::go_to_marker, "3"};
        case key::num_5:
            return action{action_kind::go_to_marker, "4"};
        case key::num_6:
            return action{action_kind::go_to_marker, "5"};
        case key::num_7:
            return action{action_kind::go_to_marker, "6"};
        case key::num_8:
            return action{action_kind::go_to_marker, "7"};
        case key::num_9:
            return action{action_kind::go_to_marker, "8"};

        case key::esc:
        case key::plus:
        case key::hyphen:
        case key::ret:
        case key::tab:
        case key::shift_tab:
        case key::up:
        case key::down:
            return std::nullopt;
    }
}

bool action_normal_routing::responds_to_action(ae::action_kind const kind) {
    switch (kind) {
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
        case action_kind::select_time_unit:
            return true;

        case action_kind::finish_time_editing:
        case action_kind::cancel_time_editing:
        case action_kind::move_to_next_time_unit:
        case action_kind::move_to_previous_time_unit:
        case action_kind::input_time:
        case action_kind::delete_time:
        case action_kind::increment_time:
        case action_kind::decrement_time:
        case action_kind::change_time_sign_to_plus:
        case action_kind::change_time_sign_to_minus:
            return false;
    }
}
