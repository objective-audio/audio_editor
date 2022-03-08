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
class file_track;
class marker_pool;
class project;
class project_editor;
class responder_stack;

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
    std::weak_ptr<project> const _project;
    std::weak_ptr<player> const _player;
    std::weak_ptr<file_track> const _file_track;
    std::weak_ptr<marker_pool> const _marker_pool;
    std::weak_ptr<project_editor> const _project_editor;
    std::weak_ptr<responder_stack> const _responder_stack;
    observing::fetcher_ptr<file_track_event> const _file_track_event_fetcher;
    observing::fetcher_ptr<marker_pool_event> const _marker_pool_event_fetcher;
    observing::canceller_pool _pool;

    editing_root_presenter(file_info const &file_info, std::shared_ptr<player> const &,
                           std::shared_ptr<file_track> const &, std::shared_ptr<marker_pool> const &,
                           std::shared_ptr<project> const &, std::shared_ptr<project_editor> const &,
                           std::shared_ptr<responder_stack> const &);

    editing_root_presenter(editing_root_presenter const &) = delete;
    editing_root_presenter(editing_root_presenter &&) = delete;
    editing_root_presenter &operator=(editing_root_presenter const &) = delete;
    editing_root_presenter &operator=(editing_root_presenter &&) = delete;
};
}  // namespace yas::ae
