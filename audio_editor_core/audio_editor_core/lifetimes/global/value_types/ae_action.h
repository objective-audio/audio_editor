//
//  ae_action.h
//

#pragma once

#include <audio_editor_core/ae_action_id.h>
#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_marker_index.hpp>
#include <audio_editor_core/ae_module_index.hpp>
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
    select_modules,
    toggle_module_selection,
    begin_marker_renaming,
    select_markers,
    toggle_marker_selection,
    begin_time_editing,
    escape,
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

using action_value = std::variant<int64_t, std::string, time::range, module_index, std::vector<module_index>,
                                  marker_index, std::vector<marker_index>>;

struct action {
    action_name name;
    std::optional<action_value> value;

    action(action_name const name, std::optional<action_value> &&value = std::nullopt);

    [[nodiscard]] int64_t const &integer_value() const;
    [[nodiscard]] std::string const &string_value() const;
    [[nodiscard]] time::range const &range_value() const;
    [[nodiscard]] module_index const &module_index_value() const;
    [[nodiscard]] std::vector<module_index> const &module_indices_value() const;
    [[nodiscard]] marker_index const &marker_index_value() const;
    [[nodiscard]] std::vector<marker_index> const &marker_indices_value() const;
    // ae_test_utilsにテスト用としてoperator==が定義されている
};
}  // namespace yas::ae

namespace yas {
ae::action_name_kind to_kind(ae::action_name const &);
}
