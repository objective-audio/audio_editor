//
//  display_space_track_range.cpp
//

#include "display_space_track_range.hpp"

#include <ae-core/project/features/display_space.h>

#include <ae-core/project/features/vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

display_space_track_range::display_space_track_range(display_space const *display_space,
                                                     vertical_scrolling const *scrolling)
    : _display_space(display_space), _scrolling(scrolling) {
}

std::optional<track_range> display_space_track_range::current() const {
    return this->_display_space->track_range(this->_scrolling->track_position());
}

std::optional<track_range> display_space_track_range::zero() const {
    return this->_display_space->track_range(this->_scrolling->position_offset());
}
