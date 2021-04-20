//
//  ae_project_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_pool_dependency.h>

namespace yas::ae {
struct project_pool {
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
