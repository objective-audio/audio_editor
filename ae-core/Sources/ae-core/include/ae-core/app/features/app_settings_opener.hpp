//
//  app_settings_opener.hpp
//

#pragma once

namespace yas::ae {
class app_settings_lifecycle;

struct app_settings_opener final {
    app_settings_opener(app_settings_lifecycle *);

    [[nodiscard]] bool can_open() const;
    void open();

   private:
    app_settings_lifecycle *const _app_settings_lifecycle;

    app_settings_opener(app_settings_opener const &) = delete;
    app_settings_opener(app_settings_opener &&) = delete;
    app_settings_opener &operator=(app_settings_opener const &) = delete;
    app_settings_opener &operator=(app_settings_opener &&) = delete;
};
}  // namespace yas::ae
