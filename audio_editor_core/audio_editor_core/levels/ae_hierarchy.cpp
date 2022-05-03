//
//  ae_hierarchy.cpp
//

#include "ae_hierarchy.h"

#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_level_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_level> const &hierarchy::app_level() {
    return app_level::global();
}

std::shared_ptr<project_level> const &hierarchy::project_level_for_id(std::string const &project_id) {
    return app_level::global()->project_level_pool->level_for_id(project_id);
}

std::shared_ptr<project_editor_level> const &hierarchy::project_editor_level_for_id(std::string const &project_id) {
    return project_level_for_id(project_id)->editor_level_pool->level();
}
