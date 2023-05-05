//
//  ae_project_guid.cpp
//

#include "ae_project_guid.h"

using namespace yas;
using namespace yas::ae;

bool project_guid::operator==(project_guid const &rhs) const {
    return this->raw_value == rhs.raw_value;
}

bool project_guid::operator!=(project_guid const &rhs) const {
    return !(*this == rhs);
}

bool project_guid::operator<(project_guid const &rhs) const {
    return this->raw_value < rhs.raw_value;
}
