//
//  ae_timing_utils.cpp
//

#include "ae_timing_utils.h"

#include <audio_editor_core/ae_math.h>

using namespace yas;
using namespace yas::ae;

uint32_t timing_utils::to_fraction_digits(timing_fraction_kind const kind, sample_rate_t const sample_rate) {
    auto const unit = to_dividing_unit(kind, sample_rate);
    return math::fraction_digits(unit);
}

uint32_t timing_utils::to_dividing_unit(timing_fraction_kind const kind, sample_rate_t const sample_rate) {
    switch (kind) {
        case timing_fraction_kind::sample:
            return sample_rate;
        case timing_fraction_kind::milisecond:
            return 1000;
        case timing_fraction_kind::frame30:
            return 30;
    }
}
