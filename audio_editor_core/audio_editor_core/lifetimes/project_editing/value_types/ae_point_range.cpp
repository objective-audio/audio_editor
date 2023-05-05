//
//  ae_point_range.cpp
//

#include "ae_point_range.hpp"

using namespace yas;
using namespace yas::ae;

ui::region point_range::region() const {
    float const min_x = std::min(this->first.x, this->second.x);
    float const min_y = std::min(this->first.y, this->second.y);
    float const width = std::abs(this->second.x - this->first.x);
    float const height = std::abs(this->second.y - this->first.y);

    return ui::region{.origin = {.x = min_x, .y = min_y}, .size = {.width = width, .height = height}};
}

point_range point_range::second_updated(ui::point const &second) const {
    return {.first = this->first, .second = second};
}

bool point_range::operator==(point_range const &rhs) const {
    return this->first == rhs.first && this->second == rhs.second;
}

bool point_range::operator!=(point_range const &rhs) const {
    return !(*this == rhs);
}
