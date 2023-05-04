//
//  ae_window_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_lifetime_id.h>

#include <filesystem>
#include <memory>

namespace yas::ae {
class app_lifetime;
class project_lifecycle;
class window_closer;
class project_path;
class window_receiver;
class project_state_holder;
class zooming_pair;
class scrolling;
class player;
class timeline_holder;
class display_space;

struct window_lifetime final {
    [[nodiscard]] static std::shared_ptr<window_lifetime> make_shared(project_lifetime_id const &,
                                                                      project_format const &,
                                                                      std::filesystem::path const &project_dir_path);

    window_lifetime(project_lifetime_id const &, project_format const &, std::filesystem::path const &project_dir_path,
                    app_lifetime const *);

    project_lifetime_id const lifetime_id;
    project_format const project_format;
    std::filesystem::path const project_directory_path;

    std::shared_ptr<project_path> const project_path;
    std::shared_ptr<display_space> const display_space;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<timeline_holder> const timeline_holder;
    std::shared_ptr<project_lifecycle> const project_lifecycle;
    std::shared_ptr<window_closer> const closer;
    std::shared_ptr<window_receiver> const receiver;

   private:
    window_lifetime(window_lifetime const &) = delete;
    window_lifetime(window_lifetime &&) = delete;
    window_lifetime &operator=(window_lifetime const &) = delete;
    window_lifetime &operator=(window_lifetime &&) = delete;
};
}  // namespace yas::ae
