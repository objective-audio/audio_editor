//
//  ae_timing.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_timing_fraction.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct timing final : timing_for_project_editor, timing_for_nudging {
    [[nodiscard]] static std::shared_ptr<timing> make_shared(sample_rate_t const);

    [[nodiscard]] sample_rate_t sample_rate() const override;

    void set_fraction(timing_fraction const) override;
    [[nodiscard]] timing_fraction fraction() const override;
    [[nodiscard]] observing::syncable observe_fraction(std::function<void(timing_fraction const &)> &&) override;

    [[nodiscard]] uint32_t unit_sample_count() const override;

   private:
    sample_rate_t const _sample_rate;
    observing::value::holder_ptr<timing_fraction> const _fraction;

    timing(sample_rate_t const);
};
}  // namespace yas::ae
