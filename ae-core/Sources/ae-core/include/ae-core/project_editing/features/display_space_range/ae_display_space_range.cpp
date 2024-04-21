//
//  ae_display_space_range.cpp
//

#include "ae_display_space_range.hpp"

#include <ae-core/global/value_types/ae_space_range.hpp>
#include <ae-core/project_editing/features/display_space_time_range/ae_display_space_time_range.hpp>
#include <ae-core/project_editing/features/display_space_track_range/ae_display_space_track_range.hpp>

using namespace yas;
using namespace yas::ae;

display_space_range::display_space_range(display_space_time_range *time_range, display_space_track_range *track_range)
    : _time_range(time_range), _track_range(track_range) {
}

std::optional<space_range> display_space_range::current() const {
    auto const time_range = this->_time_range->current();
    auto const track_range = this->_track_range->current();

    if (time_range.has_value() && track_range.has_value()) {
        return space_range{.time_range = time_range.value(), .track_range = track_range.value()};
    } else {
        return std::nullopt;
    }
}

std::optional<space_range> display_space_range::zero() const {
    auto const time_range = this->_time_range->zero();
    auto const track_range = this->_track_range->zero();

    if (time_range.has_value() && track_range.has_value()) {
        return space_range{.time_range = time_range.value(), .track_range = track_range.value()};
    } else {
        return std::nullopt;
    }
}
