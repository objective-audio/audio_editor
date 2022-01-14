//
//  ae_timing_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_timing_types.h>

namespace yas::ae::timing_utils {
uint32_t to_fraction_digits(timing_fraction_kind const, sample_rate_t const);
uint32_t to_fraction_unit(timing_fraction_kind const, sample_rate_t const);
uint32_t to_fraction_value(frame_index_t const, timing_fraction_kind const, sample_rate_t const);
timing_components to_components(frame_index_t const, timing_fraction_kind const, sample_rate_t const);
frame_index_t to_frame(timing_components const &, timing_fraction_kind const, sample_rate_t const);
timing_components adding(timing_components const &, timing_components const &, timing_fraction_kind const,
                         sample_rate_t const);
}  // namespace yas::ae::timing_utils
