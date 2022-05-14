//
//  ae_project_editor_launcher.cpp
//

#include "ae_project_editor_launcher.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_module_loader.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_launcher> project_editor_launcher::make_shared(
    project_id const &project_id, ae::file_info const &file_info, timeline_holder *updater, database *database,
    file_track *file_track, edge_holder *edge_holder, file_module_loader *file_module_loader) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    auto const &project_url = project_level->project_url;
    return std::make_shared<project_editor_launcher>(file_info, project_level->file_url, project_url->editing_file(),
                                                     project_level->player.get(), updater, database, file_track,
                                                     edge_holder, file_module_loader);
}

project_editor_launcher::project_editor_launcher(ae::file_info const &file_info, url const &file_url,
                                                 url const &editing_file_url, player *player, timeline_holder *updater,
                                                 database *database, file_track *file_track, edge_holder *edge_holder,
                                                 file_module_loader *file_module_loader)
    : _file_info(file_info),
      _file_url(file_url),
      _editing_file_url(editing_file_url),
      _player(player),
      _timeline_holder(updater),
      _database(database),
      _file_track(file_track),
      _edge_holder(edge_holder),
      _file_module_loader(file_module_loader) {
}

void project_editor_launcher::launch() {
    this->_file_module_loader->load(this->_file_url);
}
