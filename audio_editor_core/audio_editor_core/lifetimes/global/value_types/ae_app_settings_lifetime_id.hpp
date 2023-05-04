//
//  ae_app_settings_lifetime_id.hpp
//

#pragma once

#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct app_settings_lifetime_id final {
    identifier instance;

    bool operator==(app_settings_lifetime_id const &) const;
    bool operator!=(app_settings_lifetime_id const &) const;
    bool operator<(app_settings_lifetime_id const &) const;
};
}  // namespace yas::ae
