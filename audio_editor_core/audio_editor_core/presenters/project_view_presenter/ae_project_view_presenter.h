//
//  ae_project_view_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_file_track_types.h>
#include <audio_editor_core/ae_marker_pool_types.h>
#include <observing/yas_observing_umbrella.h>

#include <cstdint>
#include <functional>
#include <memory>

namespace yas::ae {
class project;
enum class project_state;

struct project_view_presenter final {
    [[nodiscard]] static std::shared_ptr<project_view_presenter> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<project_view_presenter> make_shared(std::shared_ptr<project> const &);

    [[nodiscard]] std::string const &project_id() const;
    [[nodiscard]] std::string state_text() const;
    [[nodiscard]] std::string file_info_text() const;
    [[nodiscard]] std::string player_text() const;
    [[nodiscard]] std::string play_button_text() const;
    [[nodiscard]] std::string file_track_text() const;
    [[nodiscard]] std::string marker_pool_text() const;
    [[nodiscard]] std::string time_text() const;

    bool responds_to_action(action const);
    void handle_action(action const);

    [[nodiscard]] observing::syncable observe_state_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_file_info_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_play_button_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_file_track_text(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_marker_pool_text(std::function<void(std::string const &)> &&);

   private:
    std::shared_ptr<project> _project;
    observing::fetcher_ptr<file_track_event> const _file_track_event_fetcher;
    observing::fetcher_ptr<marker_pool_event> const _marker_pool_event_fetcher;
    observing::canceller_pool _pool;

    project_view_presenter(std::shared_ptr<project> const &);

    project_view_presenter(project_view_presenter const &) = delete;
    project_view_presenter(project_view_presenter &&) = delete;
    project_view_presenter &operator=(project_view_presenter const &) = delete;
    project_view_presenter &operator=(project_view_presenter &&) = delete;
};
}  // namespace yas::ae
