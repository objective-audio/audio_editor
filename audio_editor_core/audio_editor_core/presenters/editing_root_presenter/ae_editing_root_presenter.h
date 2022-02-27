//
//  ae_editing_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>
#include <audio_editor_core/ae_file_info.h>

namespace yas::ae {
struct editing_root_presenter final {
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(
        file_info const &file_info, std::shared_ptr<player_for_editing_root_presenter> const &,
        std::shared_ptr<project_for_editing_root_presenter> const &,
        std::shared_ptr<project_editor_for_editing_root_presenter> const &,
        std::shared_ptr<action_router_for_editing_root_presenter> const &);

    [[nodiscard]] std::string state_text() const;
    [[nodiscard]] std::string file_info_text() const;
    [[nodiscard]] std::string file_track_text() const;
    [[nodiscard]] std::string marker_pool_text() const;

    [[nodiscard]] playing_line_state_t playing_line_state() const;

    [[nodiscard]] observing::syncable observe_state_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_file_track_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_marker_pool_text(std::function<void(std::string const &)> &&);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    file_info const _file_info;
    std::weak_ptr<project_for_editing_root_presenter> _project;
    std::weak_ptr<player_for_editing_root_presenter> _player;
    std::weak_ptr<project_editor_for_editing_root_presenter> _project_editor;
    std::weak_ptr<action_router_for_editing_root_presenter> _action_router;
    observing::fetcher_ptr<file_track_event> const _file_track_event_fetcher;
    observing::fetcher_ptr<marker_pool_event> const _marker_pool_event_fetcher;
    observing::canceller_pool _pool;

    editing_root_presenter(file_info const &file_info, std::shared_ptr<player_for_editing_root_presenter> const &,
                           std::shared_ptr<project_for_editing_root_presenter> const &,
                           std::shared_ptr<project_editor_for_editing_root_presenter> const &,
                           std::shared_ptr<action_router_for_editing_root_presenter> const &);

    editing_root_presenter(editing_root_presenter const &) = delete;
    editing_root_presenter(editing_root_presenter &&) = delete;
    editing_root_presenter &operator=(editing_root_presenter const &) = delete;
    editing_root_presenter &operator=(editing_root_presenter &&) = delete;
};
}  // namespace yas::ae
