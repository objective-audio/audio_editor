//
//  ae_window_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

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

struct window_lifetime final {
    [[nodiscard]] static std::shared_ptr<window_lifetime> make_shared(window_lifetime_id const &,
                                                                      project_format const &,
                                                                      std::filesystem::path const &project_dir_path);

    window_lifetime(window_lifetime_id const &, project_format const &, std::filesystem::path const &project_dir_path,
                    app_lifetime *);

    window_lifetime_id const lifetime_id;
    project_format const project_format;
    std::filesystem::path const project_directory_path;

    std::shared_ptr<project_path> const project_path;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<timeline_holder> const timeline_holder;
    std::shared_ptr<project_lifecycle> const project_lifecycle;
    std::shared_ptr<window_closer> const closer;
    std::shared_ptr<window_receiver> const receiver;
};
}  // namespace yas::ae
