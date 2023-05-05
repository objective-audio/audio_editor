//
//  ae_project_lifetime.cpp
//

#include "ae_project_lifetime.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_project_receiver.h>
#include <audio_editor_core/ae_project_sub_lifecycle.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_path.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_lifetime> project_lifetime::make_shared(project_lifetime_id const &lifetime_id,
                                                                ae::project_format const &project_format,
                                                                std::filesystem::path const &project_dir_path) {
    auto const &app_lifetime = hierarchy::app_lifetime();

    return std::make_shared<project_lifetime>(lifetime_id, project_format, project_dir_path, app_lifetime.get());
}

project_lifetime::project_lifetime(project_lifetime_id const &lifetime_id, ae::project_format const &project_format,
                                   std::filesystem::path const &project_dir_path, app_lifetime const *app_lifetime)
    : lifetime_id(lifetime_id),
      project_format(project_format),
      project_directory_path(project_dir_path),
      project_path(std::make_shared<ae::project_path>(project_dir_path)),
      display_space(std::make_shared<ae::display_space>()),
      zooming_pair(std::make_shared<ae::zooming_pair>()),
      scrolling(std::make_shared<ae::scrolling>()),
      player(std::make_shared<ae::player>(app_lifetime->system_path->playing_directory(), lifetime_id.guid,
                                          this->scrolling.get())),
      timeline_holder(std::make_shared<ae::timeline_holder>(this->project_format, this->project_path.get())),
      project_sub_lifecycle(std::make_shared<ae::project_sub_lifecycle>(lifetime_id)),
      closer(std::make_shared<ae::project_closer>(lifetime_id, app_lifetime->project_lifecycle.get(),
                                                  app_lifetime->project_settings_lifecycle.get())),
      receiver(std::make_shared<ae::project_receiver>(lifetime_id)) {
}
