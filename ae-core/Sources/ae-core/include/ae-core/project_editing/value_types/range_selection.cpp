//
//  range_selection.cpp
//

#include "range_selection.hpp"

using namespace yas;
using namespace yas::ae;

bool range_selection::operator==(range_selection const &rhs) const {
    return this->phase == rhs.phase && this->range == rhs.range;
}

bool range_selection::operator!=(range_selection const &rhs) const {
    return !(*this == rhs);
}
