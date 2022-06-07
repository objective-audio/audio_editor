//
//  ae_app_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_project_lifecycle_types.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_lifecycle_for_app_presenter {
    virtual ~project_lifecycle_for_app_presenter() = default;

    [[nodiscard]] virtual observing::syncable observe_event(
        std::function<void(project_lifecycle_event const &)> &&) = 0;
};
}  // namespace yas::ae
