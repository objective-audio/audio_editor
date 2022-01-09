//
//  ae_timing_utils.cpp
//

#include "ae_timing_utils.h"

#include <audio_editor_core/ae_math.h>

using namespace yas;
using namespace yas::ae;

uint32_t timing_utils::to_fraction_digits(timing_fraction const fraction, sample_rate_t const sample_rate) {
    auto const unit = to_dividing_unit(fraction, sample_rate);
    return math::fraction_digits(unit);
}

uint32_t timing_utils::to_dividing_unit(timing_fraction const fraction, sample_rate_t const sample_rate) {
    switch (fraction) {
        case timing_fraction::sample:
            return sample_rate;
        case timing_fraction::milisecond:
            return 1000;
        case timing_fraction::frame30:
            return 30;
    }
}
