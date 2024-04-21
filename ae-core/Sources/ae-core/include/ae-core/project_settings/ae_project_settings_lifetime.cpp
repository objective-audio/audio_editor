//
//  ae_project_settings_lifetime.cpp
//

#include "ae_project_settings_lifetime.hpp"

#include <ae-core/app/ae_app_lifetime.h>
#include <ae-core/global/ae_hierarchy.h>

#include <ae-core/project_settings/features/ae_project_settings_closer.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_settings_lifetime> project_settings_lifetime::make_shared(
    project_lifetime_id const &lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);
    return std::make_shared<project_settings_lifetime>(lifetime_id, app_lifetime.get(), project_lifetime.get());
}

project_settings_lifetime::project_settings_lifetime(project_lifetime_id const &lifetime_id,
                                                     app_lifetime const *app_lifetime,
                                                     project_lifetime const *project_lifetime)
    : lifetime_id(lifetime_id),
      project_format(project_lifetime->project_format),
      closer(std::make_shared<project_settings_closer>(lifetime_id, app_lifetime->project_settings_lifecycle.get())) {
}
