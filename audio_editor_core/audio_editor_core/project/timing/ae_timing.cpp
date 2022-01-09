//
//  ae_timing.cpp
//

#include "ae_timing.h"

#include <audio_editor_core/ae_timing_utils.h>
#include <playing/yas_playing_math.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<timing> timing::make_shared(sample_rate_t const sample_rate) {
    return std::shared_ptr<timing>(new timing{sample_rate});
}

timing::timing(sample_rate_t const sample_rate)
    : _sample_rate(sample_rate),
      _fraction(observing::value::holder<timing_fraction>::make_shared(timing_fraction::sample)) {
}

sample_rate_t timing::sample_rate() const {
    return this->_sample_rate;
}

void timing::set_fraction(timing_fraction const fraction) {
    this->_fraction->set_value(fraction);
}

timing_fraction timing::fraction() const {
    return this->_fraction->value();
}

observing::syncable timing::observe_fraction(std::function<void(timing_fraction const &)> &&handler) {
    return this->_fraction->observe(std::move(handler));
}

uint32_t timing::unit_sample_count() const {
    return this->_sample_rate / timing_utils::to_dividing_unit(this->_fraction->value(), this->_sample_rate);
}

uint32_t timing::fraction_digits() const {
    return timing_utils::to_fraction_digits(this->_fraction->value(), this->_sample_rate);
}

uint32_t timing::fraction_value(frame_index_t const frame) const {
    auto const mod_frame = std::abs(frame) % this->_sample_rate;
    auto const dividing = timing_utils::to_dividing_unit(this->_fraction->value(), this->_sample_rate);
    double const rate = static_cast<double>(mod_frame) / static_cast<double>(this->_sample_rate);
    return static_cast<uint32_t>(rate * dividing);
}
