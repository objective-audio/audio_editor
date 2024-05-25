//
//  app_settings_lifecycle.hpp
//

#pragma once

#include <ae-core/app/lifecycles/app_settings_lifecycle_event.h>

#include <ae-core/global/value_types/app_settings_lifetime_id.hpp>
#include <observing/umbrella.hpp>

namespace yas::ae {
class app_settings_lifetime;

struct app_settings_lifecycle final {
    app_settings_lifecycle();

    void add_lifetime();
    void remove_lifetime(app_settings_lifetime_id const &);

    [[nodiscard]] std::shared_ptr<app_settings_lifetime> const &current() const;
    [[nodiscard]] bool has_current() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(app_settings_lifecycle_event const &)> &&);

   private:
    observing::value::holder_ptr<std::shared_ptr<app_settings_lifetime>> const _current;

    app_settings_lifecycle(app_settings_lifecycle const &) = delete;
    app_settings_lifecycle(app_settings_lifecycle &&) = delete;
    app_settings_lifecycle &operator=(app_settings_lifecycle const &) = delete;
    app_settings_lifecycle &operator=(app_settings_lifecycle &&) = delete;
};
}  // namespace yas::ae
