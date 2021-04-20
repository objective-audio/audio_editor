//
//  ae_project_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_pool_dependency.h>
#include <audio_editor_core/ae_project_pool_types.h>

namespace yas::ae {
struct project_pool {
    project_ptr add_project(url const &file_url);
    [[nodiscard]] project_ptr project_for_id(uintptr_t const) const;
    [[nodiscard]] std::vector<project_ptr> projects() const;
    [[nodiscard]] observing::syncable observe_project(std::function<void(project_pool_event const &)> &&);

    static project_pool_ptr make_shared();

   private:
    using projects_map_t = observing::map::holder<uintptr_t, std::pair<project_ptr, observing::cancellable_ptr>>;
    std::shared_ptr<projects_map_t> const _projects = projects_map_t::make_shared();

    project_pool();

    project_pool(project_pool const &) = delete;
    project_pool(project_pool &&) = delete;
    project_pool &operator=(project_pool const &) = delete;
    project_pool &operator=(project_pool &&) = delete;
};
}  // namespace yas::ae
