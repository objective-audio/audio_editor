//
//  ae_settings_opener.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class settings_lifecycle;

struct settings_opener {
    settings_opener(window_lifetime_id const, settings_lifecycle *);

    [[nodiscard]] bool can_open_settings() const;

    void open_settings();

   private:
    window_lifetime_id const _lifetime_id;
    settings_lifecycle *const _lifecycle;

    settings_opener(settings_opener const &) = delete;
    settings_opener(settings_opener &&) = delete;
    settings_opener &operator=(settings_opener const &) = delete;
    settings_opener &operator=(settings_opener &&) = delete;
};
}  // namespace yas::ae
