//
//  ae_nudge_settings.cpp
//

#include "ae_nudge_settings.h"

using namespace yas;
using namespace yas::ae;

nudge_settings::nudge_settings(timing_for_nudge_settings const *timing, app_settings_for_nudge_settings *app_settings)
    : _timing(timing),
      _app_settings(app_settings),
      _kind(observing::value::holder<timing_unit_kind>::make_shared(app_settings->timing_unit_kind())) {
}

timing_unit_kind nudge_settings::kind() const {
    return this->_kind->value();
}

void nudge_settings::rotate_next_unit() {
    this->_kind->set_value(rotate_next(this->_kind->value()));
    this->_update_app_settings();
}

void nudge_settings::rotate_previous_unit() {
    this->_kind->set_value(rotate_previous(this->_kind->value()));
    this->_update_app_settings();
}

std::size_t nudge_settings::unit_index() const {
    return to_index(this->_kind->value());
}

observing::syncable nudge_settings::observe_kind(std::function<void(timing_unit_kind const &)> &&handler) {
    return this->_kind->observe(std::move(handler));
}

frame_index_t nudge_settings::next_nudging_frame(frame_index_t const current_frame, uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = offset_count,
                                                   .unit_index = this->unit_index(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    return current_frame + this->_timing->frame(offset);
}

frame_index_t nudge_settings::previous_nudging_frame(frame_index_t const current_frame,
                                                     uint32_t const offset_count) const {
    auto const current = this->_timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = true,
                                                   .count = offset_count,
                                                   .unit_index = this->unit_index(),
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
                                                   .unit_index = this->unit_index(),
                                                   .fraction_unit_size = floored.fraction_unit_size()});
    auto const next = floored.adding(offset);
    return this->_timing->frame(next);
}

void nudge_settings::_update_app_settings() {
    this->_app_settings->set_timing_unit_kind(this->_kind->value());
}
