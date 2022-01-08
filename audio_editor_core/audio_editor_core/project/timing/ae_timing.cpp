//
//  ae_timing.cpp
//

#include "ae_timing.h"

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
    switch (this->_fraction->value()) {
        case timing_fraction::sample:
            return 1;
        case timing_fraction::milisecond:
            return this->_sample_rate / 1000;
        case timing_fraction::frame30:
            return this->_sample_rate / 30;
    }
}
