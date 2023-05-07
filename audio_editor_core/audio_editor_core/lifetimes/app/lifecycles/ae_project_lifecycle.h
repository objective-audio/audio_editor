//
//  ae_project_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_project_lifecycle_event.h>
#include <observing/yas_observing_umbrella.h>

#include <filesystem>

namespace yas::ae {
class uuid_generatable;
class id_generatable;
class project_lifetime;
class uuid_generatable;
class file_info_loader;
class project_format;

struct project_lifecycle final : action_receiver_providable {
    project_lifecycle(id_generatable const *, uuid_generatable const *);

    void add_lifetime(std::filesystem::path const &project_dir_path, project_format const &);
    void remove_lifetime(project_lifetime_id const &);

    [[nodiscard]] std::shared_ptr<project_lifetime> const &lifetime_for_id(project_lifetime_id const &) const;
    [[nodiscard]] std::shared_ptr<project_lifetime> const &lifetime_for_path(std::filesystem::path const &) const;

    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(project_lifecycle_event const &)> &&);

   private:
    id_generatable const *const _id_generator;
    uuid_generatable const *const _uuid_generator;

    using lifetimes_t = observing::map::holder<project_lifetime_id, std::shared_ptr<project_lifetime>>;
    std::shared_ptr<lifetimes_t> const _lifetimes = lifetimes_t::make_shared();

    project_lifecycle(project_lifecycle const &) = delete;
    project_lifecycle(project_lifecycle &&) = delete;
    project_lifecycle &operator=(project_lifecycle const &) = delete;
    project_lifecycle &operator=(project_lifecycle &&) = delete;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
