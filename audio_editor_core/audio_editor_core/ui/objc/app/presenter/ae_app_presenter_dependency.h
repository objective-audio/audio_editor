//
//  ae_app_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_window_lifecycle_event.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct window_lifecycle_for_app_presenter {
    virtual ~window_lifecycle_for_app_presenter() = default;

    [[nodiscard]] virtual observing::syncable observe_event(std::function<void(window_lifecycle_event const &)> &&) = 0;
};
}  // namespace yas::ae
