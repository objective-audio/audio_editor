//
//  ae_edge.cpp
//

#include "ae_edge.h"

using namespace yas;
using namespace yas::ae;

bool edge::operator==(edge const &rhs) const {
    return this->begin_frame == rhs.begin_frame && this->end_frame == rhs.end_frame;
}

bool edge::operator!=(edge const &rhs) const {
    return !(*this == rhs);
}

edge const &edge::zero() {
    static edge const zero_value{.begin_frame = 0, .end_frame = 0};
    return zero_value;
}

std::string yas::to_string(ae::edge const &value) {
    return "{" + std::to_string(value.begin_frame) + ", " + std::to_string(value.end_frame) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::edge const &value) {
    os << yas::to_string(value);
    return os;
}
