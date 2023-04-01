//
//  ae_nudging.cpp
//

#include "ae_nudging.h"

using namespace yas;
using namespace yas::ae;

nudging::nudging(timing_for_nudging const *timing, app_settings_for_nudging *app_settings)
    : _timing(timing),
      _app_settings(app_settings),
      _kind(observing::value::holder<timing_unit_kind>::make_shared(app_settings->timing_unit_kind())) {
}

timing_unit_kind nudging::kind() const {
    return this->_kind->value();
}

void nudging::rotate_next_unit() {
    this->_kind->set_value(rotate_next(this->_kind->value()));
    this->_update_app_settings();
}

void nudging::rotate_previous_unit() {
    this->_kind->set_value(rotate_previous(this->_kind->value()));
    this->_update_app_settings();
}

std::size_t nudging::unit_index() const {
    return to_index(this->_kind->value());
}

observing::syncable nudging::observe_kind(std::function<void(timing_unit_kind const &)> &&handler) {
    return this->_kind->observe(std::move(handler));
}

frame_index_t nudging::next_nudging_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = offset_count,
                                                   .unit_index = this->unit_index(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudging::previous_nudging_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = true,
                                                   .count = offset_count,
                                                   .unit_index = this->unit_index(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudging::current_grid_frame(frame_index_t const current_frame) const {
    auto const floored = this->_timing->floored_components(timing_unit_kind{this->unit_index()}, current_frame);
    return this->_timing->frame(floored);
}

frame_index_t nudging::next_grid_frame(frame_index_t const current_frame) const {
    auto const floored = this->_timing->floored_components(timing_unit_kind{this->unit_index()}, current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = 1,
                                                   .unit_index = this->unit_index(),
                                                   .fraction_unit_size = floored.fraction_unit_size()});
    auto const next = floored.adding(offset);
    return this->_timing->frame(next);
}

void nudging::_update_app_settings() {
    this->_app_settings->set_timing_unit_kind(this->_kind->value());
}
