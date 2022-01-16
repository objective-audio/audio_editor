//
//  ae_nudging_test_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>

namespace yas::ae::test_utils {
struct timing_stub final : timing_for_nudging {
    sample_rate_t sample_rate_value = 0;
    timing_components components_value{{.fraction_unit_count = 30}};
    frame_index_t frame_value;

    timing_stub(sample_rate_t const sample_rate);

    sample_rate_t sample_rate() const override;

    timing_components components(frame_index_t const) const override;
    frame_index_t frame(timing_components const &) const override;
};
}  // namespace yas::ae::test_utils
