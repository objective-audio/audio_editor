//
//  ae_timing.cpp
//

#include "ae_timing.h"

#include <audio_editor_core/ae_timing_utils.h>
#include <playing/yas_playing_math.h>

using namespace yas;
using namespace yas::ae;

timing::timing(sample_rate_t const sample_rate, app_settings_for_timing *app_settings)
    : _sample_rate(sample_rate), _app_settings(app_settings) {
}

sample_rate_t timing::sample_rate() const {
    return this->_sample_rate;
}

void timing::rotate_fraction() {
    auto const prev_kind = this->_app_settings->timing_fraction_kind();
    this->_app_settings->set_timing_fraction_kind(rotate_next(prev_kind));
}

timing_components timing::components(frame_index_t const frame) const {
    return timing_utils::to_components(frame, this->_app_settings->timing_fraction_kind(), this->_sample_rate);
}

timing_components timing::floored_components(timing_unit_kind const unit_kind, frame_index_t const frame) const {
    return timing_utils::to_floored_components(frame, unit_kind, this->_app_settings->timing_fraction_kind(),
                                               this->_sample_rate);
}

uint32_t timing::fraction_digits() const {
    return timing_utils::to_fraction_digits(this->_app_settings->timing_fraction_kind(), this->_sample_rate);
}

frame_index_t timing::frame(timing_components const &components) const {
    return timing_utils::to_frame(components, this->_app_settings->timing_fraction_kind(), this->_sample_rate);
}
