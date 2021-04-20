//
//  ae_project_pool.h
//

#pragma once

#include <audio_editor_core/ae_project_pool_dependency.h>

namespace yas::ae {
struct project_pool {
    static project_pool_ptr make_shared();

   private:
    project_pool(project_pool const &) = delete;
    project_pool(project_pool &&) = delete;
    project_pool &operator=(project_pool const &) = delete;
    project_pool &operator=(project_pool &&) = delete;
};
}  // namespace yas::ae
