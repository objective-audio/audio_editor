//
//  ae_index_range.cpp
//

#include "ae_index_range.h"

using namespace yas;
using namespace yas::ae;

bool index_range::operator==(index_range const &rhs) const {
    return this->index == rhs.index && this->length == rhs.length;
}

bool index_range::operator!=(index_range const &rhs) const {
    return !(*this == rhs);
}

std::string yas::to_string(ae::index_range const &range) {
    return "{index:" + std::to_string(range.index) + ", length:" + std::to_string(range.length) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::index_range const &value) {
    os << to_string(value);
    return os;
}
