//
//  ae_project_sub_lifetime_id.cpp
//

#include "ae_project_sub_lifetime_id.h"

using namespace yas;
using namespace yas::ae;

bool project_sub_lifetime_id::operator==(time_editor_lifetime_id const &rhs) const {
    return this->instance == rhs.instance && this->window == rhs.window;
}

bool project_sub_lifetime_id::operator!=(time_editor_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool project_sub_lifetime_id::operator<(time_editor_lifetime_id const &rhs) const {
    if (this->instance < rhs.instance) {
        return true;
    } else if (rhs.instance < this->instance) {
        return false;
    } else {
        return this->window < rhs.window;
    }
}
