//
//  ae_nudging_test_utils.cpp
//

#include "ae_nudging_test_utils.h"

#include <audio_editor_core/ae_timing_utils.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::test_utils;

timing_stub::timing_stub(sample_rate_t const sample_rate, timing_fraction_kind const kind)
    : sample_rate_value(sample_rate), fraction_kind_value(kind) {
}

sample_rate_t timing_stub::sample_rate() const {
    return this->sample_rate_value;
}

timing_components timing_stub::components(frame_index_t const frame) const {
    return timing_utils::to_components(frame, this->fraction_kind_value, this->sample_rate_value);
}

timing_components timing_stub::floored_components(timing_unit_kind const unit_kind, frame_index_t const frame) const {
    return timing_utils::to_floored_components(frame, unit_kind, this->fraction_kind_value, this->sample_rate_value);
}

frame_index_t timing_stub::frame(timing_components const &components) const {
    return timing_utils::to_frame(components, this->fraction_kind_value, this->sample_rate_value);
}
