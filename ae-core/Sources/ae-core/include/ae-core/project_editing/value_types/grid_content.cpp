//
//  grid_content.cpp
//

#include "grid_content.hpp"

using namespace yas;
using namespace yas::ae;

grid_content::grid_content(frame_index_t const frame, uint32_t const sample_rate)
    : identifier(frame), frame(frame), sample_rate(sample_rate) {
}

float grid_content::x() const {
    return static_cast<double>(this->frame) / static_cast<double>(this->sample_rate);
}

bool grid_content::operator==(grid_content const &rhs) const {
    return this->frame == rhs.frame && this->sample_rate == rhs.sample_rate;
}

bool grid_content::operator!=(grid_content const &rhs) const {
    return !(*this == rhs);
}
