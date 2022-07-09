//
//  ae_window_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class app_lifetime;
class project_lifecycle;
class window_closer;
class project_url;
class window_receiver;
class project_state_holder;
class zooming_pair;
class scrolling;

struct window_lifetime final {
    [[nodiscard]] static std::shared_ptr<window_lifetime> make_shared(window_lifetime_id const &,
                                                                      project_format const &,
                                                                      url const &project_dir_url);

    window_lifetime(window_lifetime_id const &, project_format const &, url const &project_dir_url, app_lifetime *);

    window_lifetime_id const lifetime_id;
    project_format const project_format;
    url const project_directory_url;

    std::shared_ptr<project_url> const project_url;
    std::shared_ptr<project_state_holder> const project_state_holder;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<project_lifecycle> const project_lifecycle;
    std::shared_ptr<window_closer> const closer;
    std::shared_ptr<window_receiver> const receiver;
};
}  // namespace yas::ae
