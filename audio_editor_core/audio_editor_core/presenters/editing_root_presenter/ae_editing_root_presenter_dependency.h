//
//  ae_editing_root_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_types.h>
#include <audio_editor_core/ae_timing_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player_for_project_editor;
class file_track_for_project_editor;
class marker_pool_for_project_editor;
class marker;
class marker_pool_event;

struct action_router_for_editing_root_presenter {
    virtual ~action_router_for_editing_root_presenter() = default;

    [[nodiscard]] virtual bool responds_to_action(ae::action_kind const) const = 0;
};

struct project_for_editing_root_presenter {
    virtual ~project_for_editing_root_presenter() = default;

    [[nodiscard]] virtual project_state const &state() const = 0;

    [[nodiscard]] virtual observing::syncable observe_state(std::function<void(project_state const &)> &&) = 0;
};

struct player_for_editing_root_presenter {
    virtual ~player_for_editing_root_presenter() = default;

    [[nodiscard]] virtual bool is_playing() const = 0;
};

struct project_editor_for_editing_root_presenter {
    virtual ~project_editor_for_editing_root_presenter() = default;

    [[nodiscard]] virtual frame_index_t current_frame() const = 0;
    [[nodiscard]] virtual bool is_scrolling() const = 0;

    [[nodiscard]] virtual bool can_nudge() const = 0;
    [[nodiscard]] virtual timing_fraction_kind timing_fraction_kind() const = 0;
    [[nodiscard]] virtual bool can_jump_to_previous_edge() const = 0;
    [[nodiscard]] virtual bool can_jump_to_next_edge() const = 0;
    [[nodiscard]] virtual bool can_jump_to_beginnig() const = 0;
    [[nodiscard]] virtual bool can_jump_to_end() const = 0;
    [[nodiscard]] virtual bool can_split() const = 0;
    [[nodiscard]] virtual bool can_erase() const = 0;
    [[nodiscard]] virtual bool can_insert_marker() const = 0;
    [[nodiscard]] virtual bool can_return_to_zero() const = 0;
    [[nodiscard]] virtual bool can_go_to_marker(std::size_t const) const = 0;
    [[nodiscard]] virtual bool can_undo() const = 0;
    [[nodiscard]] virtual bool can_redo() const = 0;
    [[nodiscard]] virtual bool can_select_file_for_export() const = 0;
    [[nodiscard]] virtual bool can_cut() const = 0;
    [[nodiscard]] virtual bool can_copy() const = 0;
    [[nodiscard]] virtual bool can_paste() const = 0;
    [[nodiscard]] virtual bool can_begin_time_editing() const = 0;
    [[nodiscard]] virtual bool can_end_time_editing() const = 0;

    [[nodiscard]] virtual std::map<frame_index_t, marker> const &markers() const = 0;
    [[nodiscard]] virtual file_track_module_map_t const &modules() const = 0;

    [[nodiscard]] virtual observing::syncable observe_file_track_event(
        std::function<void(file_track_event const &)> &&) = 0;
    [[nodiscard]] virtual observing::syncable observe_marker_pool_event(
        std::function<void(marker_pool_event const &)> &&) = 0;
    [[nodiscard]] virtual observing::syncable observe_timing_fraction(
        std::function<void(ae::timing_fraction_kind const &)> &&) = 0;
};
}  // namespace yas::ae
