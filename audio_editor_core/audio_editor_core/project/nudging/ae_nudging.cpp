//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

#include <audio_editor_core/ae_nudging_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudging> nudging::make_shared(std::shared_ptr<timing_for_nudging> const &timing) {
    return std::shared_ptr<nudging>(new nudging{timing});
}

nudging::nudging(std::shared_ptr<timing_for_nudging> const &timing)
    : _timing(timing),
      _kind(observing::value::holder<nudging_kind>::make_shared(nudging_kind::fraction)),
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

frame_index_t nudging::next_frame(frame_index_t const frame) const {
    auto const current = this->_timing->components(frame);
    auto const offset = nudging_utils::offset_components(false, this->_kind->value());
    auto const next = this->_timing->adding(current, offset);
    return this->_timing->frame(next);
}

frame_index_t nudging::previous_frame(frame_index_t const frame) const {
    auto const current = this->_timing->components(frame);
    auto const offset = nudging_utils::offset_components(true, this->_kind->value());
    auto const next = this->_timing->adding(current, offset);
    return this->_timing->frame(next);
}
