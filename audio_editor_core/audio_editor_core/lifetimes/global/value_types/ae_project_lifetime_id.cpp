//
//  ae_project_lifetime_id.cpp
//

#include "ae_project_lifetime_id.h"

using namespace yas;
using namespace yas::ae;

bool project_lifetime_id::operator==(project_lifetime_id const &rhs) const {
    return this->instance == rhs.instance && this->guid == rhs.guid;
}

bool project_lifetime_id::operator!=(project_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool project_lifetime_id::operator<(project_lifetime_id const &rhs) const {
    if (this->instance < rhs.instance) {
        return true;
    } else if (rhs.instance < this->instance) {
        return false;
    } else {
        return this->guid < rhs.guid;
    }
}
