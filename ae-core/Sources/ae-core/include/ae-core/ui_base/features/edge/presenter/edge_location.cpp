//
//  edge_location.cpp
//

#include "edge_location.h"

using namespace yas;
using namespace yas::ae;

namespace yas::ae::edge_location_utils {
float to_x(frame_index_t const &frame, uint32_t const sample_rate, ui::size const &scale) {
    return static_cast<double>(frame) / static_cast<double>(sample_rate) * static_cast<double>(scale.width);
}
}  // namespace yas::ae::edge_location_utils

edge_location::edge_location(frame_index_t const &frame, uint32_t const sample_rate, ui::size const &scale)
    : edge_location(edge_location_utils::to_x(frame, sample_rate, scale)) {
}

edge_location::edge_location(float const x) : x(x) {
}

bool edge_location::operator==(edge_location const &rhs) const {
    return this->x == rhs.x;
}

bool edge_location::operator!=(edge_location const &rhs) const {
    return !(*this == rhs);
}

bool edge_locations::operator==(edge_locations const &rhs) const {
    return this->begin == rhs.begin && this->end == rhs.end;
}

bool edge_locations::operator!=(edge_locations const &rhs) const {
    return !(*this == rhs);
}
