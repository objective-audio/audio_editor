//
//  ae_ui_hierarchy.mm
//

#include "ae_ui_hierarchy.h"
#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/app/lifecycles/ae_ui_resource_lifecycle.hpp>
#include <ae-core/ui_resource/lifecycles/ae_ui_base_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_resource_lifetime> const &ui_hierarchy::resource_lifetime_for_project_lifetime_id(
    project_lifetime_id const &lifetime_id) {
    return hierarchy::app_lifetime()->ui_resource_lifecycle->lifetime_for_project_lifetime_id(lifetime_id);
}

std::shared_ptr<ui_base_lifetime> const &ui_hierarchy::base_lifetime_for_project_lifetime_id(
    project_lifetime_id const &lifetime_id) {
    auto const &resource_lifetime = resource_lifetime_for_project_lifetime_id(lifetime_id);
    return resource_lifetime->base_lifecycle->lifetime();
}
