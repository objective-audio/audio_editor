//
//  ae_project_status.h
//

#pragma once

#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_launcher_dependency.h>
#include <audio_editor_core/ae_project_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_status final : project_status_for_project_closer, project_status_for_project_launcher {
    [[nodiscard]] static std::shared_ptr<project_status> make_shared();

    [[nodiscard]] project_state const &state() const override;
    void set_state(project_state const &) override;

    [[nodiscard]] observing::syncable observe_state(std::function<void(project_state const &)> &&);

   private:
    observing::value::holder_ptr<project_state> const _state;

    project_status();
};
}  // namespace yas::ae
