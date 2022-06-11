//
//  ae_window_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_window_lifecycle_types.h>

namespace yas::ae {
class window_lifetime;
class uuid_generatable;
class file_info_loader;
class project_id;
class project_format;

struct window_lifecycle final : window_lifecycle_for_app_presenter, window_lifecycle_for_project_closer {
    [[nodiscard]] static std::shared_ptr<window_lifecycle> make_shared();

    window_lifecycle(std::shared_ptr<uuid_generatable> const &);

    void add_lifetime(url const &project_dir_url, project_id const &, project_format const &);
    void remove_lifetime(ae::project_id const &project_id) override;

    [[nodiscard]] std::shared_ptr<window_lifetime> const &lifetime_for_id(ae::project_id const &) const;

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(window_lifecycle_event const &)> &&) override;

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    using window_lifetimes_t =
        observing::map::holder<ae::project_id, std::pair<std::shared_ptr<window_lifetime>, observing::cancellable_ptr>>;
    std::shared_ptr<window_lifetimes_t> const _window_lifetimes = window_lifetimes_t::make_shared();

    window_lifecycle(window_lifecycle const &) = delete;
    window_lifecycle(window_lifecycle &&) = delete;
    window_lifecycle &operator=(window_lifecycle const &) = delete;
    window_lifecycle &operator=(window_lifecycle &&) = delete;
};
}  // namespace yas::ae
