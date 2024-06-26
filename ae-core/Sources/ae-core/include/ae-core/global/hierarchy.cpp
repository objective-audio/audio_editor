//
//  hierarchy.cpp
//

#include "hierarchy.h"

#include <ae-core/app/lifecycles/app_modal_lifecycle.h>
#include <ae-core/app/lifecycles/project_lifecycle.h>
#include <ae-core/project/lifecycles/project_sub_lifecycle.h>
#include <ae-core/project_editing/lifecycles/project_modal_lifecycle.h>
#include <cpp-utils/assertion.h>

#include <ae-core/app/lifecycles/app_settings_lifecycle.hpp>
#include <ae-core/app/lifecycles/project_settings_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<app_lifecycle> const _global_app_lifecycle = std::make_shared<app_lifecycle>();
static std::shared_ptr<app_settings_lifetime> const _empty_app_settings_lifetime = nullptr;

std::shared_ptr<app_lifecycle> const &hierarchy::app_lifecycle() {
    return _global_app_lifecycle;
}

std::shared_ptr<app_lifetime> const &hierarchy::app_lifetime() {
    return hierarchy::app_lifecycle()->lifetime();
}

std::shared_ptr<project_setup_dialog_lifetime> const &hierarchy::project_setup_dialog_lifetime() {
    return hierarchy::app_lifetime()->modal_lifecycle->project_setup_dialog_lifetime();
}

std::shared_ptr<project_lifetime> const &hierarchy::project_lifetime_for_id(project_lifetime_id const &lifetime_id) {
    return hierarchy::app_lifetime()->project_lifecycle->lifetime_for_id(lifetime_id);
}

std::shared_ptr<app_settings_lifetime> const &hierarchy::app_settings_lifetime_for_id(
    app_settings_lifetime_id const &lifetime_id) {
    auto const &current = hierarchy::app_lifetime()->app_settings_lifecycle->current();
    if (current && current->lifetime_id == lifetime_id) {
        return current;
    } else {
        return _empty_app_settings_lifetime;
    }
}

std::shared_ptr<project_settings_lifetime> const &hierarchy::project_settings_lifetime_for_id(
    project_lifetime_id const &lifetime_id) {
    return hierarchy::app_lifetime()->project_settings_lifecycle->lifetime_for_id(lifetime_id);
}

std::shared_ptr<project_editing_lifetime> const &hierarchy::project_editing_lifetime_for_id(
    project_lifetime_id const &lifetime_id) {
    return get<project_editing_lifetime>(
        hierarchy::project_lifetime_for_id(lifetime_id)->project_sub_lifecycle->current());
}

std::shared_ptr<module_name_sheet_lifetime> const &hierarchy::module_name_sheet_lifetime_for_id(
    project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.project);
    return lifetime->modal_lifecycle->module_name_sheet_lifetime();
}

std::shared_ptr<marker_name_sheet_lifetime> const &hierarchy::marker_name_sheet_lifetime_for_id(
    project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.project);
    return lifetime->modal_lifecycle->marker_name_sheet_lifetime();
}
