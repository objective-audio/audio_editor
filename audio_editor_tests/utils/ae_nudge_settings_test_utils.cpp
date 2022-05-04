//
//  ae_nudge_settings_test_utils.cpp
//

#include "ae_nudge_settings_test_utils.h"

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::test_utils;

timing_stub::timing_stub(sample_rate_t const sample_rate) : sample_rate_value(sample_rate) {
}

sample_rate_t timing_stub::sample_rate() const {
    return sample_rate_value;
}

timing_components timing_stub::components(frame_index_t const frame) const {
    return this->components_handler(frame);
}

frame_index_t timing_stub::frame(timing_components const &components) const {
    return this->frame_handler(components);
}
