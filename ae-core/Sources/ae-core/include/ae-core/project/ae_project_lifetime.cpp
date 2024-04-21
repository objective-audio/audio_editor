//
//  ae_project_lifetime.cpp
//

#include "ae_project_lifetime.h"

#include <ae-core/app/features/ae_system_path.h>
#include <ae-core/global/ae_hierarchy.h>
#include <ae-core/project/features/ae_display_space.h>
#include <ae-core/project/features/ae_horizontal_scrolling.h>
#include <ae-core/project/features/ae_player.h>
#include <ae-core/project/features/ae_project_closer.h>
#include <ae-core/project/features/ae_project_path.h>
#include <ae-core/project/features/ae_project_receiver.h>
#include <ae-core/project/features/ae_timeline_holder.h>
#include <ae-core/project/features/ae_zooming_pair.h>
#include <ae-core/project/lifecycles/ae_project_sub_lifecycle.h>

#include <ae-core/project/features/ae_vertical_scrolling.hpp>

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
      horizontal_scrolling(std::make_shared<ae::horizontal_scrolling>()),
      vertical_scrolling(std::make_shared<ae::vertical_scrolling>()),
      player(std::make_shared<ae::player>(app_lifetime->system_path->playing_directory(), lifetime_id.guid,
                                          this->horizontal_scrolling.get())),
      timeline_holder(std::make_shared<ae::timeline_holder>(this->project_format, this->project_path.get())),
      project_sub_lifecycle(std::make_shared<ae::project_sub_lifecycle>(lifetime_id)),
      closer(std::make_shared<ae::project_closer>(lifetime_id, app_lifetime->project_lifecycle.get(),
                                                  app_lifetime->project_settings_lifecycle.get())),
      receiver(std::make_shared<ae::project_receiver>(lifetime_id)) {
}
