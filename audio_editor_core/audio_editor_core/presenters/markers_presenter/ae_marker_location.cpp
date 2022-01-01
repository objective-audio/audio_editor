//
//  ae_marker_location.cpp
//

#include "ae_marker_location.h"

using namespace yas;
using namespace yas::ae;

marker_location marker_location::make_value(ae::identifier const &identifier, proc::frame_index_t const &frame,
                                            uint32_t const sample_rate, ui::size const &scale) {
    double const position = static_cast<double>(frame) / static_cast<double>(sample_rate);
    return marker_location{.identifier = identifier,
                           .x = static_cast<float>(position),
                           .point = {.x = static_cast<float>(position * scale.width), .y = scale.height * 0.5f}};
}

bool marker_location::operator==(marker_location const &rhs) const {
    return this->identifier == rhs.identifier && this->point == rhs.point && this->x == rhs.x;
}

bool marker_location::operator!=(marker_location const &rhs) const {
    return !(*this == rhs);
}
