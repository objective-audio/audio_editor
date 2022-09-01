//
//  ae_window_lifetime.cpp
//

#include "ae_window_lifetime.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_path.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_window_closer.h>
#include <audio_editor_core/ae_window_receiver.h>
#include <audio_editor_core/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<window_lifetime> window_lifetime::make_shared(window_lifetime_id const &lifetime_id,
                                                              ae::project_format const &project_format,
                                                              std::filesystem::path const &project_dir_path) {
    auto const &app_lifetime = hierarchy::app_lifetime();

    return std::make_shared<window_lifetime>(lifetime_id, project_format, project_dir_path, app_lifetime.get());
}

window_lifetime::window_lifetime(window_lifetime_id const &lifetime_id, ae::project_format const &project_format,
                                 std::filesystem::path const &project_dir_path, app_lifetime *app_lifetime)
    : lifetime_id(lifetime_id),
      project_format(project_format),
      project_directory_path(project_dir_path),
      project_path(ae::project_path::make_shared(project_dir_path)),
      zooming_pair(zooming_pair::make_shared()),
      scrolling(scrolling::make_shared()),
      player(player::make_shared(app_lifetime->system_path->playing_directory(), lifetime_id.project,
                                 this->scrolling.get())),
      timeline_holder(timeline_holder::make_shared(this->project_format, this->project_path.get())),
      project_lifecycle(ae::project_lifecycle::make_shared(lifetime_id)),
      closer(ae::window_closer::make_shared(lifetime_id, app_lifetime->window_lifecycle.get())),
      receiver(ae::window_receiver::make_shared(lifetime_id)) {
}
