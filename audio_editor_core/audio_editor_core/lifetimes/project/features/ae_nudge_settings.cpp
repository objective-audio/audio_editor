//
//  ae_nudge_settings.cpp
//

#include "ae_nudge_settings.h"

using namespace yas;
using namespace yas::ae;

nudge_settings::nudge_settings(timing_for_nudge_settings const *timing)
    : _timing(timing), _unit_idx(observing::value::holder<std::size_t>::make_shared(0)) {
}

void nudge_settings::rotate_next_unit() {
    auto const &idx = this->_unit_idx->value();
    if (idx == 0) {
        auto const components = this->_timing->components(0).raw_components();
        this->_unit_idx->set_value(components.size() - 1);
    } else {
        this->_unit_idx->set_value(idx - 1);
    }
}

void nudge_settings::rotate_previous_unit() {
    auto const idx = this->_unit_idx->value() + 1;
    auto const components = this->_timing->components(0).raw_components();
    if (components.size() <= idx) {
        this->_unit_idx->set_value(0);
    } else {
        this->_unit_idx->set_value(idx);
    }
}

std::size_t nudge_settings::unit_index() const {
    return this->_unit_idx->value();
}

observing::syncable nudge_settings::observe_unit_index(std::function<void(std::size_t const &)> &&handler) {
    return this->_unit_idx->observe(std::move(handler));
}

frame_index_t nudge_settings::next_nudging_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudge_settings::previous_nudging_frame(frame_index_t const current_frame,
                                                     uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = true,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudge_settings::current_grid_frame(frame_index_t const current_frame) const {
    auto const floored = this->_timing->floored_components(timing_unit_kind{this->unit_index()}, current_frame);
    return this->_timing->frame(floored);
}

frame_index_t nudge_settings::next_grid_frame(frame_index_t const current_frame) const {
    auto const floored = this->_timing->floored_components(timing_unit_kind{this->unit_index()}, current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = 1,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = floored.fraction_unit_size()});
    auto const next = floored.adding(offset);
    return this->_timing->frame(next);
}
