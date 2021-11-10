//
//  ae_root_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_project_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_for_root_presenter {
    virtual ~project_for_root_presenter() = default;

    [[nodiscard]] virtual observing::syncable observe_state(std::function<void(project_state const &)> &&) = 0;
};
}  // namespace yas::ae
