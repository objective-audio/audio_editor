//
//  ae_marker_location.cpp
//

#include "ae_marker_location.h"

using namespace yas;
using namespace yas::ae;

namespace yas::ae::marker_location_utils {
static float to_x(frame_index_t const &frame, uint32_t const sample_rate, float const scale) {
    double const position = static_cast<double>(frame) / static_cast<double>(sample_rate);
    return static_cast<float>(position * scale);
}
}  // namespace yas::ae::marker_location_utils

marker_location::marker_location(yas::identifier const &identifier, frame_index_t const &frame,
                                 uint32_t const sample_rate, float const scale)
    : marker_location(identifier, marker_location_utils::to_x(frame, sample_rate, scale)) {
}

marker_location::marker_location(yas::identifier const &identifier, float const x) : identifier(identifier), x(x) {
}

bool marker_location::operator==(marker_location const &rhs) const {
    return this->identifier == rhs.identifier && this->x == rhs.x;
}

bool marker_location::operator!=(marker_location const &rhs) const {
    return !(*this == rhs);
}
