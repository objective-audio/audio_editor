//
//  ae_window_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_url.h>

#include <memory>

namespace yas::ae {
class project_lifecycle;
class window_closer;
class project_url;

struct window_lifetime final {
    [[nodiscard]] static std::shared_ptr<window_lifetime> make_shared(ae::project_id const &, project_format const &,
                                                                      url const &project_dir_url);

    window_lifetime(ae::project_id const &, project_format const &, url const &project_dir_url,
                    std::shared_ptr<project_url> const &, std::shared_ptr<project_lifecycle> const &,
                    std::shared_ptr<window_closer> const &);

    ae::project_id const project_id;
    project_format const project_format;
    url const project_directory_url;

    std::shared_ptr<project_url> const project_url;
    std::shared_ptr<project_lifecycle> const project_lifecycle;
    std::shared_ptr<window_closer> const closer;
};
}  // namespace yas::ae
