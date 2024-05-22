//
//  ae_app_settings_closer.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_app_settings_lifetime_id.hpp>

namespace yas::ae {
class app_settings_lifecycle;

struct app_settings_closer final {
    app_settings_closer(app_settings_lifetime_id const &, app_settings_lifecycle *);

    [[nodiscard]] bool can_close() const;
    void close_if_needed();

   private:
    app_settings_lifetime_id const _lifetime_id;
    app_settings_lifecycle *_lifecycle;

    app_settings_closer(app_settings_closer const &) = delete;
    app_settings_closer(app_settings_closer &&) = delete;
    app_settings_closer &operator=(app_settings_closer const &) = delete;
    app_settings_closer &operator=(app_settings_closer &&) = delete;
};
}  // namespace yas::ae
