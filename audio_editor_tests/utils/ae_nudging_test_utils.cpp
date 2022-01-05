//
//  ae_nudging_test_utils.cpp
//

#include "ae_nudging_test_utils.h"

using namespace yas;
using namespace yas::ae;

test_utils::timing_stub::timing_stub(sample_rate_t const sample_rate, uint32_t const unit_sample_count)
    : sample_rate_value(sample_rate), unit_sample_count_value(unit_sample_count) {
}

sample_rate_t test_utils::timing_stub::sample_rate() const {
    return sample_rate_value;
}

uint32_t test_utils::timing_stub::unit_sample_count() const {
    return unit_sample_count_value;
}
