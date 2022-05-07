//
//  ae_timing.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <audio_editor_core/ae_timing_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct timing final : timing_for_project_editor {
    [[nodiscard]] static std::shared_ptr<timing> make_shared(sample_rate_t const);

    [[nodiscard]] sample_rate_t sample_rate() const override;

    void set_fraction_kind(timing_fraction_kind const) override;
    void rotate_fraction();
    [[nodiscard]] timing_fraction_kind fraction_kind() const override;
    [[nodiscard]] observing::syncable observe_fraction_kind(
        std::function<void(timing_fraction_kind const &)> &&) override;

    [[nodiscard]] timing_components components(frame_index_t const) const override;
    [[nodiscard]] uint32_t fraction_digits() const;

    [[nodiscard]] frame_index_t frame(timing_components const &) const override;

   private:
    sample_rate_t const _sample_rate;
    observing::value::holder_ptr<timing_fraction_kind> const _fraction_kind;

    timing(sample_rate_t const);
};
}  // namespace yas::ae
