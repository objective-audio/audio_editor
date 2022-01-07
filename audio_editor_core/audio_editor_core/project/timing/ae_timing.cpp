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
      _fragment(observing::value::holder<timing_fragment>::make_shared(timing_fragment::sample)) {
}

sample_rate_t timing::sample_rate() const {
    return this->_sample_rate;
}

void timing::set_fragment(timing_fragment const fragment) {
    this->_fragment->set_value(fragment);
}

timing_fragment timing::fragment() const {
    return this->_fragment->value();
}

observing::syncable timing::observe_fragment(std::function<void(timing_fragment const &)> &&handler) {
    return this->_fragment->observe(std::move(handler));
}

uint32_t timing::unit_sample_count() const {
    switch (this->_fragment->value()) {
        case timing_fragment::sample:
            return 1;
        case timing_fragment::milisecond:
            return this->_sample_rate / 1000;
        case timing_fragment::frame30:
            return this->_sample_rate / 30;
    }
}
