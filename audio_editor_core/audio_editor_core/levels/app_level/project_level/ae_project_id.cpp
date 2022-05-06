//
//  ae_project_id.cpp
//

#include "ae_project_id.h"

using namespace yas;
using namespace yas::ae;

bool project_id::operator==(project_id const &rhs) const {
    return this->raw_value == rhs.raw_value;
}

bool project_id::operator!=(project_id const &rhs) const {
    return !(*this == rhs);
}

bool project_id::operator<(project_id const &rhs) const {
    return this->raw_value < rhs.raw_value;
}
