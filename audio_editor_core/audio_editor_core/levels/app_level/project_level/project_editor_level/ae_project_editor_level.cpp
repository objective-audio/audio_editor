//
//  ae_project_editor_level.cpp
//

#include "ae_project_editor_level.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_url.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level> project_editor_level::make_shared(ae::project_id const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::make_shared<project_editor_level>(project_id, project_level->project_format, project_level->project_url,
                                                  project_level);
}

project_editor_level::project_editor_level(ae::project_id const &project_id, ae::project_format const &project_format,
                                           std::shared_ptr<project_url> const &project_url,
                                           std::shared_ptr<project_level> const &project_level)
    : project_id(project_id) {
}
