//
//  ae_app_settings.hpp
//

#pragma once

#include <ae-core/project_editing/features/ae_nudging_dependencies.h>
#include <ae-core/project_editing/features/ae_timing_dependencies.h>
#include <ae-core/project_editing/value_types/ae_timing_types.h>

namespace yas::ae {
struct app_settings final : app_settings_for_nudging, app_settings_for_timing {
    app_settings();

    void set_timing_fraction_kind(ae::timing_fraction_kind const) override;
    [[nodiscard]] ae::timing_fraction_kind timing_fraction_kind() const override;
    [[nodiscard]] observing::syncable observe_timing_fraction_kind(
        std::function<void(ae::timing_fraction_kind const &)> &&) override;

    void set_timing_unit_kind(ae::timing_unit_kind const) override;
    [[nodiscard]] ae::timing_unit_kind timing_unit_kind() const override;
    [[nodiscard]] observing::syncable observe_timing_unit_kind(
        std::function<void(ae::timing_unit_kind const &)> &&) override;

   private:
    observing::fetcher_ptr<ae::timing_fraction_kind> _timing_fraction_kind_fetcher;
    observing::fetcher_ptr<ae::timing_unit_kind> _timing_unit_kind_fetcher;

    app_settings(app_settings const &) = delete;
    app_settings(app_settings &&) = delete;
    app_settings &operator=(app_settings const &) = delete;
    app_settings &operator=(app_settings &&) = delete;
};
}  // namespace yas::ae
