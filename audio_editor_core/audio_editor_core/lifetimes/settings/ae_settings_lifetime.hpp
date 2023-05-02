//
//  ae_settings_lifetime.hpp
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class settings_closer;
class app_lifetime;
class window_lifetime;

struct settings_lifetime final {
    static std::shared_ptr<settings_lifetime> make_shared(window_lifetime_id const &);
    settings_lifetime(window_lifetime_id const &, app_lifetime const *, window_lifetime const *);

    window_lifetime_id const lifetime_id;

    project_format const project_format;
    std::shared_ptr<settings_closer> const closer;

   private:
    settings_lifetime(settings_lifetime const &) = delete;
    settings_lifetime(settings_lifetime &&) = delete;
    settings_lifetime &operator=(settings_lifetime const &) = delete;
    settings_lifetime &operator=(settings_lifetime &&) = delete;
};
}  // namespace yas::ae
