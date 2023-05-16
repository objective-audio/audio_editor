//
//  ae_project_launcher.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_presenter_dependency.h>

#include <observing/yas_observing_umbrella.hpp>

namespace yas::ae {
class player;
class timeline_holder;
class project_sub_lifecycle;

struct project_launcher final : std::enable_shared_from_this<project_launcher> {
    project_launcher(project_format const &, player *, timeline_holder const *, project_sub_lifecycle *);

    void launch();

   private:
    project_format const _project_format;

    player *const _player;
    timeline_holder const *const _timeline_holder;
    project_sub_lifecycle *const _project_sub_lifecycle;

    observing::canceller_pool _pool;

    project_launcher(project_launcher const &) = delete;
    project_launcher(project_launcher &&) = delete;
    project_launcher &operator=(project_launcher const &) = delete;
    project_launcher &operator=(project_launcher &&) = delete;
};
}  // namespace yas::ae
