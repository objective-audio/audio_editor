//
//  ae_time_editor_lifetime_id.cpp
//

#include "ae_time_editor_lifetime_id.h"

using namespace yas;
using namespace yas::ae;

bool time_editor_lifetime_id::operator==(time_editor_lifetime_id const &rhs) const {
    return this->instance == rhs.instance && this->window == rhs.window;
}

bool time_editor_lifetime_id::operator!=(time_editor_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool time_editor_lifetime_id::operator<(time_editor_lifetime_id const &rhs) const {
    if (this->instance < rhs.instance) {
        return true;
    } else if (rhs.instance < this->instance) {
        return false;
    } else {
        return this->window < rhs.window;
    }
}
