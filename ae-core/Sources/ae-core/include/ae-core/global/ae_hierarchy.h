//
//  ae_hierarchy.h
//

#pragma once

#include <ae-core/app/ae_app_lifetime.h>
#include <ae-core/global/lifecycles/ae_app_lifecycle.h>
#include <ae-core/project/ae_project_lifetime.h>
#include <ae-core/project_editing/ae_project_editing_lifetime.h>
#include <ae-core/project_format_dialog/ae_project_setup_dialog_lifetime.h>

#include <ae-core/app_settings/ae_app_settings_lifetime.hpp>
#include <ae-core/marker_name_sheet/ae_marker_name_sheet_lifetime.hpp>
#include <ae-core/module_name_sheet/ae_module_name_sheet_lifetime.hpp>
#include <ae-core/project_settings/ae_project_settings_lifetime.hpp>

namespace yas::ae {
struct hierarchy final {
    [[nodiscard]] static std::shared_ptr<app_lifecycle> const &app_lifecycle();

    [[nodiscard]] static std::shared_ptr<app_lifetime> const &app_lifetime();
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_lifetime> const &project_setup_dialog_lifetime();
    [[nodiscard]] static std::shared_ptr<project_lifetime> const &project_lifetime_for_id(project_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<app_settings_lifetime> const &app_settings_lifetime_for_id(
        app_settings_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<project_settings_lifetime> const &project_settings_lifetime_for_id(
        project_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<project_editing_lifetime> const &project_editing_lifetime_for_id(
        project_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<module_name_sheet_lifetime> const &module_name_sheet_lifetime_for_id(
        project_sub_lifetime_id const &);
    [[nodiscard]] static std::shared_ptr<marker_name_sheet_lifetime> const &marker_name_sheet_lifetime_for_id(
        project_sub_lifetime_id const &);
};
}  // namespace yas::ae
