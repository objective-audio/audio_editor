//
//  ae_action.h
//

#pragma once

#include <string>

namespace yas::ae {
enum class action_kind {
    toggle_play,
    nudge_previous,
    nudge_next,
    rotate_nudging_kind,
    rotate_timing_fraction,
    jump_previous,
    jump_next,
    jump_to_beginning,
    jump_to_end,
    drop_head_and_offset,
    split,
    drop_tail_and_offset,
    erase_and_offset,
    insert_marker,
    return_to_zero,
    go_to_marker,
    undo,
    redo,
    select_file_for_export,
    cut,
    copy,
    paste,

    begin_time_editing,
    finish_time_editing,
    cancel_time_editing,
    move_to_next_time_unit,
    move_to_previous_time_unit,
    input_time,
    delete_time,
    change_time_sign_to_plus,
    change_time_sign_to_minus,
    select_time_unit,
};

struct action {
    action_kind kind;
    std::string value;

    action(action_kind const kind) : action(kind, "") {
    }

    action(action_kind const kind, std::string const &value) : kind(kind), value(value) {
    }
};
}  // namespace yas::ae
