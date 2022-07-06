//
//  ae_project_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <audio_editor_core/ae_project_state.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>

namespace yas::ae {
class project_editor_responder;
class player;
class timeline_holder;

struct project_launcher final : std::enable_shared_from_this<project_launcher> {
    [[nodiscard]] static std::shared_ptr<project_launcher> make_shared(project_format const &,
                                                                       project_state_holder_for_project_launcher *,
                                                                       player *, timeline_holder const *);

    project_launcher(project_format const &, project_state_holder_for_project_launcher *, player *,
                     timeline_holder const *);

    void launch();

   private:
    project_format const _project_format;

    project_state_holder_for_project_launcher *const _state_holder;
    player *const _player;
    timeline_holder const *const _timeline_holder;

    observing::canceller_pool _pool;

    project_launcher(project_launcher const &) = delete;
    project_launcher(project_launcher &&) = delete;
    project_launcher &operator=(project_launcher const &) = delete;
    project_launcher &operator=(project_launcher &&) = delete;
};
}  // namespace yas::ae
