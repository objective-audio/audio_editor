//
//  ae_project_launching_lifetime.cpp
//

#include "ae_project_launching_lifetime.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_window_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launching_lifetime> project_launching_lifetime::make_shared(
    ae::project_lifetime_id const &project_lifetime_id) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(project_lifetime_id);
    return std::make_shared<project_launching_lifetime>(window_lifetime.get());
}

project_launching_lifetime::project_launching_lifetime(ae::window_lifetime *window_lifetime)
    : project_lifetime_id(window_lifetime->lifetime_id),
      launcher(std::make_shared<ae::project_launcher>(window_lifetime->project_format, window_lifetime->player.get(),
                                                      window_lifetime->timeline_holder.get(),
                                                      window_lifetime->project_lifecycle.get())) {
}
