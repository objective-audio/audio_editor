//
//  ae_window_closer.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class window_lifecycle;
class project_settings_lifecycle;

struct window_closer final {
    window_closer(window_lifetime_id const &, window_lifecycle *, project_settings_lifecycle *);

    [[nodiscard]] bool can_close() const;
    void close_if_needed();

   private:
    window_lifetime_id const _lifetime_id;
    window_lifecycle *_window_lifecycle;
    project_settings_lifecycle *_project_settings_lifecycle;

    window_closer(window_closer const &) = delete;
    window_closer(window_closer &&) = delete;
    window_closer &operator=(window_closer const &) = delete;
    window_closer &operator=(window_closer &&) = delete;
};
}  // namespace yas::ae
