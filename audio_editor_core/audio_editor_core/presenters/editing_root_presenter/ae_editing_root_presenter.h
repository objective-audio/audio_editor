//
//  ae_editing_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>

namespace yas::ae {
struct editing_root_presenter final {
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(
        std::shared_ptr<project_for_editing_root_presenter> const &,
        std::shared_ptr<project_editor_for_editing_root_presenter> const &,
        std::shared_ptr<action_router_for_editing_root_presenter> const &);

    [[nodiscard]] std::string const &project_id() const;
    [[nodiscard]] std::string state_text() const;
    [[nodiscard]] std::string file_info_text() const;
    [[nodiscard]] std::string play_button_text() const;
    [[nodiscard]] std::string file_track_text() const;
    [[nodiscard]] std::string marker_pool_text() const;
    [[nodiscard]] std::string nudge_text() const;
    [[nodiscard]] std::string timing_text() const;

    [[nodiscard]] bool is_play_button_enabled() const;
    [[nodiscard]] bool is_split_button_enabled() const;
    [[nodiscard]] bool is_drop_head_and_offset_button_enabled() const;
    [[nodiscard]] bool is_drop_tail_and_offset_button_enabled() const;
    [[nodiscard]] bool is_jump_previous_button_enabled() const;
    [[nodiscard]] bool is_jump_next_button_enabled() const;
    [[nodiscard]] bool is_erase_and_offset_button_enabled() const;
    [[nodiscard]] bool is_insert_marker_button_enabled() const;
    [[nodiscard]] bool is_zero_button_enabled() const;
    [[nodiscard]] bool is_undo_button_enabled() const;
    [[nodiscard]] bool is_redo_button_enabled() const;
    [[nodiscard]] bool is_export_button_enabled() const;
    [[nodiscard]] bool is_nudge_button_enabled() const;
    [[nodiscard]] bool is_timing_button_enabled() const;

    [[nodiscard]] playing_line_state_t playing_line_state() const;

    [[nodiscard]] observing::syncable observe_state_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_play_button_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_file_track_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_marker_pool_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_nudging_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_timing_text(std::function<void(std::string const &)> &&);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    std::weak_ptr<project_for_editing_root_presenter> _project;
    std::weak_ptr<project_editor_for_editing_root_presenter> _project_editor;
    std::weak_ptr<action_router_for_editing_root_presenter> _action_router;
    observing::fetcher_ptr<file_track_event> const _file_track_event_fetcher;
    observing::fetcher_ptr<marker_pool_event> const _marker_pool_event_fetcher;
    observing::canceller_pool _pool;

    editing_root_presenter(std::shared_ptr<project_for_editing_root_presenter> const &,
                           std::shared_ptr<project_editor_for_editing_root_presenter> const &,
                           std::shared_ptr<action_router_for_editing_root_presenter> const &);

    editing_root_presenter(editing_root_presenter const &) = delete;
    editing_root_presenter(editing_root_presenter &&) = delete;
    editing_root_presenter &operator=(editing_root_presenter const &) = delete;
    editing_root_presenter &operator=(editing_root_presenter &&) = delete;
};
}  // namespace yas::ae
