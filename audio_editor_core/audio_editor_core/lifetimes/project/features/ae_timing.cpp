//
//  ae_timing.cpp
//

#include "ae_timing.h"

#include <audio_editor_core/ae_timing_utils.h>
#include <playing/yas_playing_math.h>

using namespace yas;
using namespace yas::ae;

timing::timing(sample_rate_t const sample_rate, app_settings_for_timing *app_settings)
    : _sample_rate(sample_rate),
      _app_settings(app_settings),
      _fraction_kind(
          observing::value::holder<timing_fraction_kind>::make_shared(app_settings->timing_fraction_kind())) {
}

sample_rate_t timing::sample_rate() const {
    return this->_sample_rate;
}

void timing::set_fraction_kind(timing_fraction_kind const kind) {
    this->_fraction_kind->set_value(kind);
}

void timing::rotate_fraction() {
    this->_fraction_kind->set_value(rotate_next(this->_fraction_kind->value()));
    this->_update_app_settings();
}

timing_fraction_kind timing::fraction_kind() const {
    return this->_fraction_kind->value();
}

observing::syncable timing::observe_fraction_kind(std::function<void(timing_fraction_kind const &)> &&handler) {
    return this->_fraction_kind->observe(std::move(handler));
}

timing_components timing::components(frame_index_t const frame) const {
    return timing_utils::to_components(frame, this->_fraction_kind->value(), this->_sample_rate);
}

timing_components timing::floored_components(timing_unit_kind const unit_kind, frame_index_t const frame) const {
    return timing_utils::to_floored_components(frame, unit_kind, this->_fraction_kind->value(), this->_sample_rate);
}

uint32_t timing::fraction_digits() const {
    return timing_utils::to_fraction_digits(this->_fraction_kind->value(), this->_sample_rate);
}

frame_index_t timing::frame(timing_components const &components) const {
    return timing_utils::to_frame(components, this->_fraction_kind->value(), this->_sample_rate);
}

void timing::_update_app_settings() {
    this->_app_settings->set_timing_fraction_kind(this->fraction_kind());
}
