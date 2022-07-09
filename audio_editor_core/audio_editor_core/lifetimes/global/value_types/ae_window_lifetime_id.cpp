//
//  ae_window_lifetime_id.cpp
//

#include "ae_window_lifetime_id.h"

using namespace yas;
using namespace yas::ae;

bool window_lifetime_id::operator==(window_lifetime_id const &rhs) const {
    return this->instance == rhs.instance && this->project == rhs.project;
}

bool window_lifetime_id::operator!=(window_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool window_lifetime_id::operator<(window_lifetime_id const &rhs) const {
    if (this->instance < rhs.instance) {
        return true;
    } else if (rhs.instance < this->instance) {
        return false;
    } else {
        return this->project < rhs.project;
    }
}
