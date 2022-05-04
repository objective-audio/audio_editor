//
//  ae_nudge_settings.cpp
//

#include "ae_nudge_settings.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudge_settings> nudge_settings::make_shared(std::shared_ptr<timing_for_nudge_settings> const &timing) {
    return std::shared_ptr<nudge_settings>(new nudge_settings{timing});
}

nudge_settings::nudge_settings(std::shared_ptr<timing_for_nudge_settings> const &timing)
    : _timing(timing), _unit_idx(observing::value::holder<std::size_t>::make_shared(0)) {
}

void nudge_settings::rotate_next_unit() {
    auto const timing = this->_timing.lock();
    if (!timing) {
        return;
    }

    auto const &idx = this->_unit_idx->value();
    if (idx == 0) {
        auto const components = timing->components(0).raw_components();
        this->_unit_idx->set_value(components.size() - 1);
    } else {
        this->_unit_idx->set_value(idx - 1);
    }
}

void nudge_settings::rotate_previous_unit() {
    auto const timing = this->_timing.lock();
    if (!timing) {
        return;
    }

    auto const idx = this->_unit_idx->value() + 1;
    auto const components = timing->components(0).raw_components();
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

std::optional<frame_index_t> nudge_settings::next_frame(frame_index_t const current_frame,
                                                        uint32_t const offset_count) const {
    auto const timing = this->_timing.lock();
    if (!timing) {
        return std::nullopt;
    }

    auto const current = timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = false,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    auto const next = current.adding(offset);
    return timing->frame(next);
}

std::optional<frame_index_t> nudge_settings::previous_frame(frame_index_t const current_frame,
                                                            uint32_t const offset_count) const {
    auto const timing = this->_timing.lock();
    if (!timing) {
        return std::nullopt;
    }

    auto const current = timing->components(current_frame);
    auto const offset = timing_components::offset({.is_minus = true,
                                                   .count = offset_count,
                                                   .unit_index = this->_unit_idx->value(),
                                                   .fraction_unit_size = current.fraction_unit_size()});
    auto const previous = current.adding(offset);
    return timing->frame(previous);
}
