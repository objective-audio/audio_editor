//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

using namespace yas;
using namespace yas::ae;

nudging::nudging(timing_for_nudging const *timing, app_settings_for_nudging *app_settings)
    : _timing(timing), _app_settings(app_settings) {
}

void nudging::rotate_next_unit() {
    auto const prev_kind = this->_app_settings->timing_unit_kind();
    this->_app_settings->set_timing_unit_kind(rotate_next(prev_kind));
}

void nudging::rotate_previous_unit() {
    auto const prev_kind = this->_app_settings->timing_unit_kind();
    this->_app_settings->set_timing_unit_kind(rotate_previous(prev_kind));
}

std::size_t nudging::_unit_index() const {
    return to_index(this->_app_settings->timing_unit_kind());
}

frame_index_t nudging::next_nudging_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_index(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudging::previous_nudging_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = true,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_index(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudging::current_grid_frame(frame_index_t const current_frame) const {
    auto const floored = this->_timing->floored_components(timing_unit_kind{this->_unit_index()}, current_frame);
    return this->_timing->frame(floored);
}

frame_index_t nudging::next_grid_frame(frame_index_t const current_frame) const {
    auto const floored = this->_timing->floored_components(timing_unit_kind{this->_unit_index()}, current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = 1,
                                                   .unit_index = this->_unit_index(),
                                                   .fraction_unit_size = floored.fraction_unit_size()});
    auto const next = floored.adding(offset);
    return this->_timing->frame(next);
}
