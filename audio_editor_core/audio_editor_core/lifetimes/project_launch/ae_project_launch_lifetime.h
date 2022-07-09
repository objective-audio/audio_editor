//
//  ae_project_launch_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ae {
class project_launcher;
class window_lifetime;

struct project_launch_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_launch_lifetime> make_shared(window_lifetime_id const &);

    project_launch_lifetime(window_lifetime *);

    window_lifetime_id const window_lifetime_id;

    std::shared_ptr<project_launcher> const launcher;
};
}  // namespace yas::ae
