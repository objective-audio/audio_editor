//
//  ae_editing_root_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_types.h>
#include <observing/yas_observing_umbrella.h>
#include <processing/yas_processing_common_types.h>

namespace yas::ae {
class player_for_project_editor;
class file_track_for_project_editor;
class marker_pool_for_project_editor;
class marker;
class marker_pool_event;

struct project_for_editing_root_presenter {
    virtual ~project_for_editing_root_presenter() = default;

    [[nodiscard]] virtual std::string const &identifier() const = 0;
    [[nodiscard]] virtual project_state const &state() const = 0;

    [[nodiscard]] virtual observing::syncable observe_state(std::function<void(project_state const &)> &&) = 0;
};

struct project_editor_for_editing_root_presenter {
    virtual ~project_editor_for_editing_root_presenter() = default;

    [[nodiscard]] virtual ae::file_info const &file_info() const = 0;

    [[nodiscard]] virtual proc::frame_index_t current_frame() const = 0;
    [[nodiscard]] virtual bool is_playing() const = 0;
    [[nodiscard]] virtual bool is_scrolling() const = 0;

    [[nodiscard]] virtual bool can_nudge() const = 0;
    [[nodiscard]] virtual nudging_kind nudging_kind() const = 0;
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

    [[nodiscard]] virtual std::map<proc::frame_index_t, marker> const &markers() const = 0;
    [[nodiscard]] virtual file_track_module_map_t const &modules() const = 0;

    [[nodiscard]] virtual observing::syncable observe_is_playing(std::function<void(bool const &)> &&) = 0;
    [[nodiscard]] virtual observing::syncable observe_file_track_event(
        std::function<void(file_track_event const &)> &&) = 0;
    [[nodiscard]] virtual observing::syncable observe_marker_pool_event(
        std::function<void(marker_pool_event const &)> &&) = 0;
    [[nodiscard]] virtual observing::syncable observe_nudging_kind(
        std::function<void(ae::nudging_kind const &)> &&) = 0;
};
}  // namespace yas::ae
