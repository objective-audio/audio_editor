//
//  ae_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_app_level_router.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_project_level.h>

namespace yas::ae {
struct hierarchy final {
    [[nodiscard]] static std::shared_ptr<app_level_router> const &app_level_router();
    [[nodiscard]] static std::shared_ptr<app_level> const &app_level();
    [[nodiscard]] static std::shared_ptr<project_level> const &project_level_for_id(project_id const &);
};
}  // namespace yas::ae
