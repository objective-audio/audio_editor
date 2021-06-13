//
//  ae_project_view_presenter.h
//

#pragma once

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
    [[nodiscard]] std::string state_string() const;
    [[nodiscard]] std::string file_info_string() const;
    [[nodiscard]] std::string player_string() const;
    [[nodiscard]] std::string play_button_string() const;
    [[nodiscard]] std::string file_track_string() const;

    void play_button_clicked();
    void split_button_clicked();
    void drop_head_button_clicked();
    void drop_tail_button_clicked();

    [[nodiscard]] observing::syncable observe_state_string(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_file_info_string(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_play_button_string(std::function<void(std::string const &)> &&);
    [[nodiscard]] observing::syncable observe_file_track_string(std::function<void(std::string const &)> &&);

   private:
    std::shared_ptr<project> _project;

    project_view_presenter(std::shared_ptr<project> const &);

    project_view_presenter(project_view_presenter const &) = delete;
    project_view_presenter(project_view_presenter &&) = delete;
    project_view_presenter &operator=(project_view_presenter const &) = delete;
    project_view_presenter &operator=(project_view_presenter &&) = delete;
};
}  // namespace yas::ae
