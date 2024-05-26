//
//  space_range.cpp
//

#include "space_range.hpp"

using namespace yas;
using namespace yas::ae;

bool space_range::operator==(space_range const &rhs) const {
    return this->time_range == rhs.time_range && this->track_range == rhs.track_range;
}

bool space_range::operator!=(space_range const &rhs) const {
    return !(*this == rhs);
}
