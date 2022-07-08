//
//  ae_app_modal_sub_lifetime_id.h
//

#pragma once

#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct app_modal_sub_lifetime_id final {
    identifier instance;

    bool operator==(app_modal_sub_lifetime_id const &) const;
    bool operator!=(app_modal_sub_lifetime_id const &) const;
    bool operator<(app_modal_sub_lifetime_id const &) const;
};

using project_setup_dialog_lifetime_id = app_modal_sub_lifetime_id;
}  // namespace yas::ae
