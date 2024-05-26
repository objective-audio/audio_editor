//
//  project_launching_lifetime.cpp
//

#include "project_launching_lifetime.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/project_lifetime.h>
#include <ae-core/project_launching/features/project_launcher.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launching_lifetime> project_launching_lifetime::make_shared(
    ae::project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    return std::make_shared<project_launching_lifetime>(project_lifetime.get());
}

project_launching_lifetime::project_launching_lifetime(ae::project_lifetime *project_lifetime)
    : project_lifetime_id(project_lifetime->lifetime_id),
      launcher(std::make_shared<ae::project_launcher>(project_lifetime->project_format, project_lifetime->player.get(),
                                                      project_lifetime->timeline_holder.get(),
                                                      project_lifetime->project_sub_lifecycle.get())) {
}
