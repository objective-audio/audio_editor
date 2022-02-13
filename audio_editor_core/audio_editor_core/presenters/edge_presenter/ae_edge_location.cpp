//
//  ae_edge_location.cpp
//

#include "ae_edge_location.h"

using namespace yas;
using namespace yas::ae;

edge_location edge_location::make_value(frame_index_t const &frame, uint32_t const sample_rate, ui::size const &scale) {
    double const x = static_cast<double>(frame) / static_cast<double>(sample_rate) * static_cast<double>(scale.width);
    return edge_location{.x = static_cast<float>(x)};
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
