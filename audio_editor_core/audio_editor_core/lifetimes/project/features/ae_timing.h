//
//  ae_timing.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <audio_editor_core/ae_timing_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct timing final : timing_for_nudge_settings {
    timing(sample_rate_t const);

    [[nodiscard]] sample_rate_t sample_rate() const override;

    void set_fraction_kind(timing_fraction_kind const);
    void rotate_fraction();
    [[nodiscard]] timing_fraction_kind fraction_kind() const;
    [[nodiscard]] observing::syncable observe_fraction_kind(std::function<void(timing_fraction_kind const &)> &&);

    [[nodiscard]] timing_components components(frame_index_t const) const override;
    [[nodiscard]] timing_components floored_components(timing_unit_kind const, frame_index_t const) const;
    [[nodiscard]] uint32_t fraction_digits() const;

    [[nodiscard]] frame_index_t frame(timing_components const &) const override;

   private:
    sample_rate_t const _sample_rate;
    observing::value::holder_ptr<timing_fraction_kind> const _fraction_kind;

    timing(timing const &) = delete;
    timing(timing &&) = delete;
    timing &operator=(timing const &) = delete;
    timing &operator=(timing &&) = delete;
};
}  // namespace yas::ae
