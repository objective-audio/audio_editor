//
//  ae_project_launch_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

#include <memory>

namespace yas::ae {
class project_launcher;
class window_lifetime;

struct project_launch_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_launch_lifetime> make_shared(project_lifetime_id const &);

    project_launch_lifetime(window_lifetime *);

    project_lifetime_id const project_lifetime_id;

    std::shared_ptr<project_launcher> const launcher;

   private:
    project_launch_lifetime(project_launch_lifetime const &) = delete;
    project_launch_lifetime(project_launch_lifetime &&) = delete;
    project_launch_lifetime &operator=(project_launch_lifetime const &) = delete;
    project_launch_lifetime &operator=(project_launch_lifetime &&) = delete;
};
}  // namespace yas::ae
