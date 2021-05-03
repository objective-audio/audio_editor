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
    static std::shared_ptr<project_view_presenter> make_shared(std::string const &project_id);
    static std::shared_ptr<project_view_presenter> make_shared(std::shared_ptr<project> const &);

    std::string const &project_id() const;
    std::string state_string() const;
    std::string file_info_string() const;
    std::string player_string() const;
    std::string play_button_string() const;

    void play_button_clicked();

    observing::syncable observe_state_string(std::function<void(std::string const &)> &&);
    observing::syncable observe_file_info_string(std::function<void(std::string const &)> &&);
    observing::syncable observe_play_button_string(std::function<void(std::string const &)> &&);

   private:
    std::shared_ptr<project> _project;

    project_view_presenter(std::shared_ptr<project> const &);

    project_view_presenter(project_view_presenter const &) = delete;
    project_view_presenter(project_view_presenter &&) = delete;
    project_view_presenter &operator=(project_view_presenter const &) = delete;
    project_view_presenter &operator=(project_view_presenter &&) = delete;
};
}  // namespace yas::ae
