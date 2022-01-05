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
    rotate_timing_fragment,
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
};
}  // namespace yas::ae
