//
//  ae_nudging_test_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>

namespace yas::ae::test_utils {
struct timing_stub final : timing_for_nudging {
    sample_rate_t sample_rate_value = 0;
    uint32_t unit_sample_count_value = 0;

    timing_stub(sample_rate_t const sample_rate, uint32_t const unit_sample_count);

    sample_rate_t sample_rate() const override;
    uint32_t unit_sample_count() const override;
};
}  // namespace yas::ae::test_utils
