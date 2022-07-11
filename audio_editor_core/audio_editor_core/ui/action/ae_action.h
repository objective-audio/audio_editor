//
//  ae_action.h
//

#pragma once

#include <audio_editor_core/ae_action_id.h>

#include <optional>
#include <string>

namespace yas::ae {
enum class action_kind {
    toggle_play,
    nudge_previous,
    nudge_next,
    nudge_previous_more,
    nudge_next_more,
    rotate_nudging_next_unit,
    rotate_nudging_previous_unit,
    rotate_timing_fraction,
    jump_previous,
    jump_next,
    jump_to_beginning,
    jump_to_end,
    drop_head,
    split,
    drop_tail,
    erase,
    insert_marker,
    set_begin_edge,
    set_end_edge,
    return_to_zero,
    go_to_marker,
    undo,
    redo,
    purge,
    select_file_for_import,
    import_from_file,
    select_file_for_export,
    export_to_file,
    cut,
    copy,
    paste,
    begin_module_renaming,

    begin_time_editing,
    finish_time_editing,
    cancel_time_editing,
    move_to_next_time_unit,
    move_to_previous_time_unit,
    input_time,
    delete_time,
    increment_time,
    decrement_time,
    change_time_sign_to_plus,
    change_time_sign_to_minus,
    select_time_unit,
};

struct action {
    action_kind kind;
    std::string value = "";

    action(action_kind const kind, std::string const &value);

    // ae_test_utilsにテスト用としてoperator==が定義されている
};
}  // namespace yas::ae
