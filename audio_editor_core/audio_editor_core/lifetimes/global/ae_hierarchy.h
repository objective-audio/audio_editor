//
//  ae_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_app_lifecycle.h>
#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_project_setup_dialog_lifetime.h>

namespace yas::ae {
struct hierarchy final {
    [[nodiscard]] static std::shared_ptr<app_lifecycle> const &app_lifecycle();

    [[nodiscard]] static std::shared_ptr<app_lifetime> const &app_lifetime();
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_lifetime> const &project_setup_dialog_lifetime();
    [[nodiscard]] static std::shared_ptr<project_lifetime> const &project_lifetime_for_id(project_id const &);
};
}  // namespace yas::ae
