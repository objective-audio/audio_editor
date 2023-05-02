//
//  ae_settings_closer.hpp
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class settings_lifecycle;

struct settings_closer final {
    settings_closer(window_lifetime_id const &, settings_lifecycle *);

    [[nodiscard]] bool can_close() const;
    void close_if_needed();

   private:
    window_lifetime_id const _lifetime_id;
    settings_lifecycle *_lifecycle;

    settings_closer(settings_closer const &) = delete;
    settings_closer(settings_closer &&) = delete;
    settings_closer &operator=(settings_closer const &) = delete;
    settings_closer &operator=(settings_closer &&) = delete;
};
}  // namespace yas::ae
