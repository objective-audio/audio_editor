//
//  ae_action.h
//

#pragma once

namespace yas::ae {
enum class action {
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
    go_to_marker_1,
    go_to_marker_2,
    go_to_marker_3,
    go_to_marker_4,
    go_to_marker_5,
    go_to_marker_6,
    go_to_marker_7,
    go_to_marker_8,
    go_to_marker_9,
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
    input_time_0,
    input_time_1,
    input_time_2,
    input_time_3,
    input_time_4,
    input_time_5,
    input_time_6,
    input_time_7,
    input_time_8,
    input_time_9,
    delete_time,
    change_time_sign_to_plus,
    change_time_sign_to_minus,
};
}  // namespace yas::ae
