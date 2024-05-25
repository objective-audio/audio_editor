//
//  edge.cpp
//

#include "edge.h"

using namespace yas;
using namespace yas::ae;

static edge const _zero_value{0, 0};

edge::edge(frame_index_t const begin_frame, frame_index_t const end_frame)
    : begin_frame(begin_frame), end_frame(end_frame) {
}

bool edge::operator==(edge const &rhs) const {
    return this->begin_frame == rhs.begin_frame && this->end_frame == rhs.end_frame;
}

bool edge::operator!=(edge const &rhs) const {
    return !(*this == rhs);
}

std::optional<time::range> edge::range() const {
    if (this->begin_frame < this->end_frame) {
        return time::range{this->begin_frame, static_cast<length_t>(this->end_frame - this->begin_frame)};
    } else {
        return std::nullopt;
    }
}

edge const &edge::zero() {
    return _zero_value;
}

std::string yas::to_string(ae::edge const &value) {
    return "{" + std::to_string(value.begin_frame) + ", " + std::to_string(value.end_frame) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::edge const &value) {
    os << yas::to_string(value);
    return os;
}
