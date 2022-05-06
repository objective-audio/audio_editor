//
//  ae_project_level_collector.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_closer_dependency.h>
#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_level_collector_types.h>

namespace yas::ae {
class project_level;
class uuid_generatable;

struct project_level_collector final : project_pool_for_app_presenter, project_level_collector_for_project_closer {
    void add_level(url const &file_url) override;
    std::shared_ptr<project_level> add_and_return_level(url const &file_url);
    void remove_level(ae::project_id const &project_id) override;
    [[nodiscard]] std::shared_ptr<project_level> const &level_for_id(ae::project_id const &) const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] observing::syncable observe_event(
        std::function<void(project_level_collector_event const &)> &&) override;

    [[nodiscard]] static std::shared_ptr<project_level_collector> make_shared();
    [[nodiscard]] static std::shared_ptr<project_level_collector> make_shared(
        std::shared_ptr<uuid_generatable> const &);

   private:
    std::shared_ptr<uuid_generatable> const _uuid_generator;

    using project_levels_t =
        observing::map::holder<ae::project_id, std::pair<std::shared_ptr<project_level>, observing::cancellable_ptr>>;
    std::shared_ptr<project_levels_t> const _project_levels = project_levels_t::make_shared();

    project_level_collector(std::shared_ptr<uuid_generatable> const &);

    project_level_collector(project_level_collector const &) = delete;
    project_level_collector(project_level_collector &&) = delete;
    project_level_collector &operator=(project_level_collector const &) = delete;
    project_level_collector &operator=(project_level_collector &&) = delete;
};
}  // namespace yas::ae
