//
//  ae_app_modal_sub_lifetime_id.cpp
//

#include "ae_app_modal_sub_lifetime_id.h"

using namespace yas;
using namespace yas::ae;

bool app_modal_sub_lifetime_id::operator==(app_modal_sub_lifetime_id const &rhs) const {
    return this->instance == rhs.instance;
}

bool app_modal_sub_lifetime_id::operator!=(app_modal_sub_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool app_modal_sub_lifetime_id::operator<(app_modal_sub_lifetime_id const &rhs) const {
    return this->instance < rhs.instance;
}
