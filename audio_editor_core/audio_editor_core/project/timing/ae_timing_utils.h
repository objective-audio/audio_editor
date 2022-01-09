//
//  ae_timing_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_timing_fraction.h>

namespace yas::ae::timing_utils {
uint32_t to_fraction_digits(timing_fraction const, sample_rate_t const);
uint32_t to_dividing_unit(timing_fraction const, sample_rate_t const);
}  // namespace yas::ae::timing_utils
