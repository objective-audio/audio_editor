//
//  ae_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_project_level.h>

namespace yas::ae {
struct hierarchy final {
    [[nodiscard]] static std::shared_ptr<app_level> const &app_level();
    [[nodiscard]] static std::shared_ptr<project_level> const &project_level_for_id(std::string const &);
    [[nodiscard]] static std::shared_ptr<project_editor_level> const &project_editor_level_for_id(std::string const &);
};
}  // namespace yas::ae
