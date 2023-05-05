//
//  ae_app_presenter_dependency.h
//

#pragma once

#include <audio_editor_core/ae_app_settings_lifecycle_event.h>
#include <audio_editor_core/ae_project_lifecycle_event.h>
#include <audio_editor_core/ae_project_settings_lifecycle_event.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class app_settings_lifetime;

struct project_lifecycle_for_app_presenter {
    virtual ~project_lifecycle_for_app_presenter() = default;

    [[nodiscard]] virtual observing::syncable observe_event(
        std::function<void(project_lifecycle_event const &)> &&) = 0;
};

struct project_settings_lifecycle_for_app_presenter {
    virtual ~project_settings_lifecycle_for_app_presenter() = default;

    [[nodiscard]] virtual observing::syncable observe_event(
        std::function<void(project_settings_lifecycle_event const &)> &&) = 0;
};

struct app_settings_lifecycle_for_app_presenter {
    virtual ~app_settings_lifecycle_for_app_presenter() = default;

    virtual void add_lifetime() = 0;
    [[nodiscard]] virtual bool has_current() const = 0;
    [[nodiscard]] virtual observing::syncable observe_event(
        std::function<void(app_settings_lifecycle_event const &)> &&) = 0;
};
}  // namespace yas::ae
