//
//  ae_hierarchy.cpp
//

#include "ae_hierarchy.h"

#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_level_router.h>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<app_level> const _global_app_level = app_level::make_shared();

std::shared_ptr<app_level> const &hierarchy::app_level() {
    return _global_app_level;
}

std::shared_ptr<project_level> const &hierarchy::project_level_for_id(project_id const &project_id) {
    return hierarchy::app_level()->project_level_router->level_for_id(project_id);
}

std::shared_ptr<project_editor_level> const &hierarchy::project_editor_level_for_id(project_id const &project_id) {
    return project_level_for_id(project_id)->editor_level_pool->level();
}
