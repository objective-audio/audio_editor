//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudging> nudging::make_shared(proc::sample_rate_t const sample_rate) {
    return std::shared_ptr<nudging>(new nudging{sample_rate});
}

nudging::nudging(proc::sample_rate_t const sample_rate)
    : _sample_rate(sample_rate), _kind(nudging_kind::sample), _unit_count(1) {
}

void nudging::set_kind(nudging_kind const kind) {
    this->_kind = kind;
}

nudging_kind nudging::kind() const {
    return this->_kind;
}

void nudging::set_unit_count(uint32_t const count) {
    this->_unit_count = count;
}

uint32_t nudging::unit_count() const {
    return this->_unit_count;
}
