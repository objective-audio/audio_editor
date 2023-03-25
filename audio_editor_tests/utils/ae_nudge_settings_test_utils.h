//
//  ae_nudge_settings_test_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>

namespace yas::ae::test_utils {
struct timing_stub final : timing_for_nudge_settings {
    sample_rate_t sample_rate_value = 0;
    timing_fraction_kind fraction_kind_value = timing_fraction_kind::frame30;

    timing_stub(sample_rate_t const sample_rate, timing_fraction_kind const kind);

    sample_rate_t sample_rate() const override;

    timing_components components(frame_index_t const) const override;
    timing_components floored_components(timing_unit_kind const, frame_index_t const) const override;
    frame_index_t frame(timing_components const &) const override;
};
}  // namespace yas::ae::test_utils
