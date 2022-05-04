//
//  ae_nudge_settings_test_utils.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>

namespace yas::ae::test_utils {
struct timing_stub final : timing_for_nudge_settings {
    sample_rate_t sample_rate_value = 0;
    std::function<timing_components(frame_index_t const &)> components_handler = [](frame_index_t const &) {
        return timing_components{{.fraction_unit_size = 30}};
    };
    std::function<frame_index_t(timing_components const &)> frame_handler = [](timing_components const &) { return 0; };

    timing_stub(sample_rate_t const sample_rate);

    sample_rate_t sample_rate() const override;

    timing_components components(frame_index_t const) const override;
    frame_index_t frame(timing_components const &) const override;
};
}  // namespace yas::ae::test_utils
