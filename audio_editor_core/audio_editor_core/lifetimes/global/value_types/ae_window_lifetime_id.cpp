//
//  ae_window_lifetime_id.cpp
//

#include "ae_window_lifetime_id.h"

using namespace yas;
using namespace yas::ae;

bool window_lifetime_id::operator==(window_lifetime_id const &rhs) const {
    return this->instance_id == rhs.instance_id && this->project_id == rhs.project_id;
}

bool window_lifetime_id::operator!=(window_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool window_lifetime_id::operator<(window_lifetime_id const &rhs) const {
    return this->instance_id < rhs.instance_id && this->project_id < rhs.project_id;
}
