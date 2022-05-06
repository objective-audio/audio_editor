//
//  ae_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_project_level.h>

namespace yas::ae::hierarchy {
[[nodiscard]] std::shared_ptr<app_level> const &app_level();
[[nodiscard]] std::shared_ptr<project_level> const &project_level_for_id(project_id const &);
[[nodiscard]] std::shared_ptr<project_editor_level> const &project_editor_level_for_id(project_id const &);
}  // namespace yas::ae::hierarchy
