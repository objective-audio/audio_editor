//
//  ae_window_presenter.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <audio_editor_core/ae_window_presenter_dependency.h>
#include <audio_editor_core/ae_window_presenter_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class project_url;
class window_closer;
class project_lifecycle;

struct window_presenter final {
    [[nodiscard]] static std::shared_ptr<window_presenter> make_shared(window_lifetime_id const &);

    window_presenter(ae::window_lifetime_id const &, std::shared_ptr<project_url> const &,
                     std::shared_ptr<window_closer> const &, std::shared_ptr<project_lifecycle> const &);

    window_lifetime_id const lifetime_id;

    std::string title() const;

    bool should_close();

    [[nodiscard]] observing::syncable observe(std::function<void(window_presenter_event const &)> &&);

   private:
    std::weak_ptr<project_url> const _project_url;
    std::weak_ptr<window_closer> _closer;
    std::weak_ptr<project_lifecycle> const _project_lifecycle;

    window_presenter(window_presenter const &) = delete;
    window_presenter(window_presenter &&) = delete;
    window_presenter &operator=(window_presenter const &) = delete;
    window_presenter &operator=(window_presenter &&) = delete;
};
}  // namespace yas::ae
