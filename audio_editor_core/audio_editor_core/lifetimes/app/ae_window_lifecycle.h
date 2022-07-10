//
//  ae_window_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_window_lifecycle_types.h>

namespace yas::ae {
class uuid_generatable;
class id_generatable;
class window_lifetime;
class uuid_generatable;
class file_info_loader;
class project_format;

struct window_lifecycle final : window_lifecycle_for_app_presenter, action_receiver_providable {
    [[nodiscard]] static std::shared_ptr<window_lifecycle> make_shared();

    window_lifecycle(std::shared_ptr<id_generatable> const &, std::shared_ptr<uuid_generatable> const &);

    void add_lifetime(url const &project_dir_url, project_format const &);
    void remove_lifetime(window_lifetime_id const &);

    [[nodiscard]] std::shared_ptr<window_lifetime> const &lifetime_for_id(window_lifetime_id const &) const;

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(window_lifecycle_event const &)> &&) override;

   private:
    std::shared_ptr<id_generatable> const _id_generator;
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    using window_lifetimes_t =
        observing::map::holder<window_lifetime_id,
                               std::pair<std::shared_ptr<window_lifetime>, observing::cancellable_ptr>>;
    std::shared_ptr<window_lifetimes_t> const _window_lifetimes = window_lifetimes_t::make_shared();

    window_lifecycle(window_lifecycle const &) = delete;
    window_lifecycle(window_lifecycle &&) = delete;
    window_lifecycle &operator=(window_lifecycle const &) = delete;
    window_lifecycle &operator=(window_lifecycle &&) = delete;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
