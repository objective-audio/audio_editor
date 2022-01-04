//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

#include <audio_editor_core/ae_nudging_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudging> nudging::make_shared(proc::sample_rate_t const sample_rate) {
    return std::shared_ptr<nudging>(new nudging{sample_rate});
}

nudging::nudging(proc::sample_rate_t const sample_rate)
    : _sample_rate(sample_rate),
      _kind(observing::value::holder<nudging_kind>::make_shared(nudging_kind::sample)),
      _unit_count(1) {
}

void nudging::set_kind(nudging_kind const kind) {
    this->_kind->set_value(kind);
}

nudging_kind nudging::kind() const {
    return this->_kind->value();
}

observing::syncable nudging::observe_kind(std::function<void(nudging_kind const &)> &&handler) {
    return this->_kind->observe(std::move(handler));
}

void nudging::set_unit_count(uint32_t const count) {
    assert(count > 0);
    this->_unit_count = count;
}

uint32_t nudging::unit_count() const {
    return this->_unit_count;
}

uint32_t nudging::unit_sample_count() const {
    return nudging_utils::to_sample_count(this->_kind->value(), this->_sample_rate) * this->_unit_count;
}
