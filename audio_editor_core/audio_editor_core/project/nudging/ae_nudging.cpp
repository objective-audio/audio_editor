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
    : _timing(timing), _unit_idx(observing::value::holder<std::size_t>::make_shared(0)), _offset_count(1) {
}

void nudging::rotate_unit_index() {
    auto const &idx = this->_unit_idx->value();
    if (idx == 0) {
        auto const components = this->_timing->components(0).raw_components();
        this->_unit_idx->set_value(components.size() - 1);
    } else {
        this->_unit_idx->set_value(idx - 1);
    }
}

std::size_t nudging::unit_index() const {
    return this->_unit_idx->value();
}

observing::syncable nudging::observe_unit_index(std::function<void(std::size_t const &)> &&handler) {
    return this->_unit_idx->observe(std::move(handler));
}

void nudging::set_offset_count(uint32_t const count) {
    assert(count > 0);
    this->_offset_count = count;
}

uint32_t nudging::offset_count() const {
    return this->_offset_count;
}

frame_index_t nudging::next_frame(frame_index_t const frame) const {
    auto const current = this->_timing->components(frame);
    auto const offset = nudging_utils::offset_components(false, this->_offset_count, this->_unit_idx->value(),
                                                         current.fraction_unit_size());
    auto const next = current.adding(offset);
    return this->_timing->frame(next);
}

frame_index_t nudging::previous_frame(frame_index_t const frame) const {
    auto const current = this->_timing->components(frame);
    auto const offset = nudging_utils::offset_components(true, this->_offset_count, this->_unit_idx->value(),
                                                         current.fraction_unit_size());
    auto const next = current.adding(offset);
    return this->_timing->frame(next);
}
