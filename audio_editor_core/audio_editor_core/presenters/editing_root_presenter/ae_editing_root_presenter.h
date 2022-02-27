//
//  ae_editing_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_editing_root_presenter_dependency.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class player;
class project;
class project_editor;
class action_router;

struct editing_root_presenter final {
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(std::string const &project_id);

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
    std::weak_ptr<project> _project;
    std::weak_ptr<player> _player;
    std::weak_ptr<project_editor> _project_editor;
    std::weak_ptr<action_router> _action_router;
    observing::fetcher_ptr<file_track_event> const _file_track_event_fetcher;
    observing::fetcher_ptr<marker_pool_event> const _marker_pool_event_fetcher;
    observing::canceller_pool _pool;

    editing_root_presenter(file_info const &file_info, std::shared_ptr<player> const &,
                           std::shared_ptr<project> const &, std::shared_ptr<project_editor> const &,
                           std::shared_ptr<action_router> const &);

    editing_root_presenter(editing_root_presenter const &) = delete;
    editing_root_presenter(editing_root_presenter &&) = delete;
    editing_root_presenter &operator=(editing_root_presenter const &) = delete;
    editing_root_presenter &operator=(editing_root_presenter &&) = delete;
};
}  // namespace yas::ae
