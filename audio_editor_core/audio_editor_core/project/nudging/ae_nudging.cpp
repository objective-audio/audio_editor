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
    : _timing(timing), _unit_idx(observing::value::holder<std::size_t>::make_shared(0)) {
}

void nudging::rotate_next_unit() {
    auto const &idx = this->_unit_idx->value();
    if (idx == 0) {
        auto const components = this->_timing->components(0).raw_components();
        this->_unit_idx->set_value(components.size() - 1);
    } else {
        this->_unit_idx->set_value(idx - 1);
    }
}

void nudging::rotate_previous_unit() {
    auto const idx = this->_unit_idx->value() + 1;
    auto const components = this->_timing->components(0).raw_components();
    if (components.size() <= idx) {
        this->_unit_idx->set_value(0);
    } else {
        this->_unit_idx->set_value(idx);
    }
}

std::size_t nudging::unit_index() const {
    return this->_unit_idx->value();
}

observing::syncable nudging::observe_unit_index(std::function<void(std::size_t const &)> &&handler) {
    return this->_unit_idx->observe(std::move(handler));
}

frame_index_t nudging::next_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    auto const next = current.adding(offset);
    return this->_timing->frame(next);
}

frame_index_t nudging::previous_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = true,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    auto const previous = current.adding(offset);
    return this->_timing->frame(previous);
}
