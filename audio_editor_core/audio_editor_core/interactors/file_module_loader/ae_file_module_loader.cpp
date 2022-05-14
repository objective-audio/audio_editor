//
//  ae_file_module_loader.cpp
//

#include "ae_file_module_loader.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_file_module_loading_state_holder.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_module_loader> file_module_loader::make_shared(project_id const &project_id,
                                                                    file_module_loading_state_holder *state_holder,
                                                                    database *database, file_track *file_track,
                                                                    edge_holder *edge_holder,
                                                                    timeline_holder const *timeline_holder) {
    auto const &app_level = hierarchy::app_level();
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_id, project_level->project_url.get(), project_level->project_format,
                       app_level->file_importer.get(), app_level->file_info_loader.get(), project_level->player.get(),
                       state_holder, database, file_track, edge_holder, timeline_holder);
}

std::shared_ptr<file_module_loader> file_module_loader::make_shared(
    project_id const &project_id, project_url const *project_url, project_format const &project_format,
    file_importer *file_importer, file_info_loader const *file_info_loader, player *player,
    file_module_loading_state_holder *state_holder, database *database, file_track *file_track,
    edge_holder *edge_holder, timeline_holder const *timeline_holder) {
    return std::make_shared<file_module_loader>(project_id, project_url, project_format, file_importer,
                                                file_info_loader, player, state_holder, database, file_track,
                                                edge_holder, timeline_holder);
}

file_module_loader::file_module_loader(project_id const &project_id, project_url const *project_url,
                                       project_format const &project_format, file_importer *file_importer,
                                       file_info_loader const *file_info_loader, player *player,
                                       file_module_loading_state_holder *state_holder, database *database,
                                       file_track *file_track, edge_holder *edge_holder,
                                       timeline_holder const *timeline_holder)
    : _project_id(project_id),
      _project_url(project_url),
      _project_format(project_format),
      _file_importer(file_importer),
      _file_info_loader(file_info_loader),
      _player(player),
      _state_holder(state_holder),
      _database(database),
      _file_track(file_track),
      _edge_holder(edge_holder),
      _timeline_holder(timeline_holder) {
}

void file_module_loader::load(url const &src_url) {
    if (this->_state_holder->state() != file_module_loading_state::waiting) {
        assertion_failure_if_not_test();
        return;
    }

    this->_state_holder->set_state(file_module_loading_state::loading);

    this->_file_importer->import(
        {.project_id = this->_project_id,
         .src_url = src_url,
         .dst_url = this->_project_url->editing_file(),
         .project_format = this->_project_format,
         .completion = [weak = this->weak_from_this()](bool const result) {
             auto const loader = weak.lock();
             if (!loader) {
                 assertion_failure_if_not_test();
                 return;
             }

             auto const editing_file_url = loader->_project_url->editing_file();
             if (auto const file_info = loader->_file_info_loader->load_file_info(editing_file_url)) {
                 loader->_database->suspend_saving([&loader, &file_info = file_info.value(), &editing_file_url] {
                     loader->_file_track->insert_module_and_notify(
                         file_module{.name = editing_file_url.last_path_component(),
                                     .range = time::range{0, file_info.length},
                                     .file_frame = 0});

                     loader->_edge_holder->set_edge(
                         {.begin_frame = 0, .end_frame = static_cast<frame_index_t>(file_info.length)});
                 });

                 loader->_player->set_timeline(loader->_timeline_holder->timeline(), file_info.value().sample_rate,
                                               audio::pcm_format::float32);
                 loader->_player->begin_rendering();
             }

             loader->_state_holder->set_state(file_module_loading_state::loaded);
         }});
}
