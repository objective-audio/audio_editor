//
//  ae_track_range.cpp
//

#include "ae_track_range.hpp"

using namespace yas;
using namespace yas::ae;

bool track_range::operator==(track_range const &rhs) const {
    return this->min == rhs.min && this->max == rhs.max;
}

bool track_range::operator!=(track_range const &rhs) const {
    return !(*this == rhs);
}

bool track_range::contains(track_index_t const index) const {
    return this->min <= index && index <= max;
}
