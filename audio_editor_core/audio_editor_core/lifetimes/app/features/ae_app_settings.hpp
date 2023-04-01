//
//  ae_app_settings.hpp
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <audio_editor_core/ae_timing_dependencies.h>
#include <audio_editor_core/ae_timing_types.h>

#include <memory>

namespace yas::ae {
struct app_settings final : app_settings_for_nudge_settings, app_settings_for_timing {
    app_settings();

    void set_timing_fraction_kind(ae::timing_fraction_kind const) override;
    [[nodiscard]] ae::timing_fraction_kind timing_fraction_kind() const override;

    void set_timing_unit_kind(ae::timing_unit_kind const) override;
    [[nodiscard]] ae::timing_unit_kind timing_unit_kind() const override;

   private:
    app_settings(app_settings const &) = delete;
    app_settings(app_settings &&) = delete;
    app_settings &operator=(app_settings const &) = delete;
    app_settings &operator=(app_settings &&) = delete;
};
}  // namespace yas::ae
