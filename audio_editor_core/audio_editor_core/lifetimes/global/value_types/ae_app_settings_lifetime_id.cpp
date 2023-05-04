//
//  ae_app_settings_lifetime_id.cpp
//

#include "ae_app_settings_lifetime_id.hpp"

using namespace yas;
using namespace yas::ae;

bool app_settings_lifetime_id::operator==(app_settings_lifetime_id const &rhs) const {
    return this->instance == rhs.instance;
}

bool app_settings_lifetime_id::operator!=(app_settings_lifetime_id const &rhs) const {
    return !(*this == rhs);
}

bool app_settings_lifetime_id::operator<(app_settings_lifetime_id const &rhs) const {
    return this->instance < rhs.instance;
}
