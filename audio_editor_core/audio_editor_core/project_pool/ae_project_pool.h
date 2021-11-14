//
//  ae_project_pool.h
//

#pragma once

#include <audio_editor_core/ae_app_presenter_dependency.h>
#include <audio_editor_core/ae_project_pool_dependency.h>
#include <audio_editor_core/ae_project_pool_types.h>

namespace yas::ae {
struct project_pool final : project_pool_for_app_presenter {
    void add_project(url const &file_url) override;
    std::shared_ptr<project_for_project_pool> add_and_return_project(url const &file_url);
    [[nodiscard]] std::shared_ptr<project_for_project_pool> project_for_id(std::string const &) const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] observing::syncable observe_event(std::function<void(project_pool_event const &)> &&) override;

    static std::shared_ptr<project_pool> make_shared();
    static std::shared_ptr<project_pool> make_shared(std::shared_ptr<project_maker_for_project_pool> const &);

   private:
    std::shared_ptr<project_maker_for_project_pool> const _project_maker;

    using projects_map_t =
        observing::map::holder<std::string,
                               std::pair<std::shared_ptr<project_for_project_pool>, observing::cancellable_ptr>>;
    std::shared_ptr<projects_map_t> const _projects = projects_map_t::make_shared();

    project_pool(std::shared_ptr<project_maker_for_project_pool> const &);

    project_pool(project_pool const &) = delete;
    project_pool(project_pool &&) = delete;
    project_pool &operator=(project_pool const &) = delete;
    project_pool &operator=(project_pool &&) = delete;
};
}  // namespace yas::ae
