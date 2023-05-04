//
//  ae_app_settings_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_app_settings_lifetime_id.hpp>

namespace yas::ae {
class app_settings_closer;
class app_lifetime;

struct app_settings_lifetime final {
    static std::shared_ptr<app_settings_lifetime> make_shared(app_settings_lifetime_id const &);
    app_settings_lifetime(app_settings_lifetime_id const &, app_lifetime const *);

    app_settings_lifetime_id const lifetime_id;

    std::shared_ptr<app_settings_closer> const closer;

   private:
    app_settings_lifetime(app_settings_lifetime const &) = delete;
    app_settings_lifetime(app_settings_lifetime &&) = delete;
    app_settings_lifetime &operator=(app_settings_lifetime const &) = delete;
    app_settings_lifetime &operator=(app_settings_lifetime &&) = delete;
};
}  // namespace yas::ae
