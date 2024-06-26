//
//  app_sub_lifetime_id.h
//

#pragma once

#include <cpp-utils/identifier.h>

namespace yas::ae {
struct app_sub_lifetime_id final {
    identifier instance;

    bool operator==(app_sub_lifetime_id const &) const;
    bool operator!=(app_sub_lifetime_id const &) const;
    bool operator<(app_sub_lifetime_id const &) const;
};

using project_setup_dialog_lifetime_id = app_sub_lifetime_id;
}  // namespace yas::ae
