//
//  ae_action.h
//

#pragma once

#include <audio_editor_core/ae_action_id.h>

#include <optional>
#include <string>
#include <variant>

namespace yas::ae {
enum class editing_action_name {
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
};

enum class time_editing_action_name {
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

using action_name = std::variant<editing_action_name, time_editing_action_name>;

enum class action_name_kind {
    editing,
    time_editing,
};

struct action {
    action_name name;
    std::string value = "";

    action(action_name const name, std::string const &value);

    // ae_test_utilsにテスト用としてoperator==が定義されている
};
}  // namespace yas::ae

namespace yas {
ae::action_name_kind to_kind(ae::action_name const &);
}
