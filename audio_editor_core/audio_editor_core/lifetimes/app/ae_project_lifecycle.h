//
//  ae_project_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_lifecycle_types.h>

namespace yas::ae {
class project_lifetime;
class uuid_generatable;
class file_info_loader;
class project_id;
class project_format;

struct project_lifecycle final : project_lifecycle_for_app_presenter, project_lifecycle_for_project_closer {
    [[nodiscard]] static std::shared_ptr<project_lifecycle> make_shared();

    project_lifecycle(std::shared_ptr<uuid_generatable> const &);

    void add_lifetime(url const &file_url, project_id const &, project_format const &);
    void remove_lifetime(ae::project_id const &project_id) override;

    [[nodiscard]] std::shared_ptr<project_lifetime> const &lifetime_for_id(ae::project_id const &) const;

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(project_lifecycle_event const &)> &&) override;

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    using project_lifetimes_t =
        observing::map::holder<ae::project_id,
                               std::pair<std::shared_ptr<project_lifetime>, observing::cancellable_ptr>>;
    std::shared_ptr<project_lifetimes_t> const _project_lifetimes = project_lifetimes_t::make_shared();

    project_lifecycle(project_lifecycle const &) = delete;
    project_lifecycle(project_lifecycle &&) = delete;
    project_lifecycle &operator=(project_lifecycle const &) = delete;
    project_lifecycle &operator=(project_lifecycle &&) = delete;
};
}  // namespace yas::ae
