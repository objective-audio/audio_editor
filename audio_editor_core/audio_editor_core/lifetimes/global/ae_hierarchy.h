//
//  ae_hierarchy.h
//

#pragma once

#include <audio_editor_core/ae_app_lifecycle.h>
#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_project_setup_dialog_lifetime.h>
#include <audio_editor_core/ae_window_lifetime.h>

#include <audio_editor_core/ae_marker_name_sheet_lifetime.hpp>
#include <audio_editor_core/ae_module_name_sheet_lifetime.hpp>
#include <audio_editor_core/ae_settings_lifetime.hpp>

namespace yas::ae {
struct hierarchy final {
    [[nodiscard]] static std::shared_ptr<app_lifecycle> const &app_lifecycle();

    [[nodiscard]] static std::shared_ptr<app_lifetime> const &app_lifetime();
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_lifetime> const &project_setup_dialog_lifetime();
    [[nodiscard]] static std::shared_ptr<window_lifetime> const &window_lifetime_for_id(window_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<settings_lifetime> const &settings_lifetime_for_id(window_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<project_lifetime> const &project_lifetime_for_id(window_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<module_name_sheet_lifetime> const &module_name_sheet_lifetime_for_id(
        project_sub_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<marker_name_sheet_lifetime> const &marker_name_sheet_lifetime_for_id(
        project_sub_lifetime_id const &);
};
}  // namespace yas::ae
