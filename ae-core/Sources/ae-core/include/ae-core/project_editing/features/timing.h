//
//  timing.h
//

#pragma once

#include <ae-core/project_editing/features/nudging_dependencies.h>
#include <ae-core/project_editing/features/timing_dependencies.h>
#include <ae-core/project_editing/value_types/timing_types.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
struct timing final : timing_for_nudging {
    timing(sample_rate_t const, app_settings_for_timing *);

    [[nodiscard]] sample_rate_t sample_rate() const override;

    void rotate_fraction();

    [[nodiscard]] timing_components components(frame_index_t const) const override;
    [[nodiscard]] timing_components floored_components(timing_unit_kind const, frame_index_t const) const override;
    [[nodiscard]] uint32_t fraction_digits() const;

    [[nodiscard]] frame_index_t frame(timing_components const &) const override;

   private:
    sample_rate_t const _sample_rate;
    app_settings_for_timing *const _app_settings;

    timing(timing const &) = delete;
    timing(timing &&) = delete;
    timing &operator=(timing const &) = delete;
    timing &operator=(timing &&) = delete;
};
}  // namespace yas::ae
