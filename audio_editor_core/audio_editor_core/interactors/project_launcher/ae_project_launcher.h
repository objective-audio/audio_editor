//
//  ae_project_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <audio_editor_core/ae_project_state.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
class project_editor_responder;
class player;
class timeline_holder;
class project_lifecycle;

struct project_launcher final : std::enable_shared_from_this<project_launcher> {
    [[nodiscard]] static std::shared_ptr<project_launcher> make_shared(project_format const &, player *,
                                                                       timeline_holder const *, project_lifecycle *);

    project_launcher(project_format const &, player *, timeline_holder const *, project_lifecycle *);

    void launch();

   private:
    project_format const _project_format;

    player *const _player;
    timeline_holder const *const _timeline_holder;
    project_lifecycle *const _project_lifecycle;

    observing::canceller_pool _pool;

    project_launcher(project_launcher const &) = delete;
    project_launcher(project_launcher &&) = delete;
    project_launcher &operator=(project_launcher const &) = delete;
    project_launcher &operator=(project_launcher &&) = delete;
};
}  // namespace yas::ae
