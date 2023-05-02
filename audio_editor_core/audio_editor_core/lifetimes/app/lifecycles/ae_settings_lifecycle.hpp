//
//  ae_settings_lifecycle.hpp
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_settings_lifecycle_event.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class settings_lifetime;

struct settings_lifecycle final : settings_lifecycle_for_app_presenter {
    settings_lifecycle();

    void add_lifetime(window_lifetime_id const &);
    void remove_lifetime(window_lifetime_id const &);

    [[nodiscard]] std::shared_ptr<settings_lifetime> const &lifetime_for_id(window_lifetime_id const &) const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(settings_lifecycle_event const &)> &&) override;

   private:
    using lifetimes_t = observing::map::holder<window_lifetime_id, std::shared_ptr<settings_lifetime>>;
    std::shared_ptr<lifetimes_t> const _lifetimes;

    settings_lifecycle(settings_lifecycle const &) = delete;
    settings_lifecycle(settings_lifecycle &&) = delete;
    settings_lifecycle &operator=(settings_lifecycle const &) = delete;
    settings_lifecycle &operator=(settings_lifecycle &&) = delete;
};
}  // namespace yas::ae
