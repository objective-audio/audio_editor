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
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_module_loader> file_module_loader::make_shared(
    project_id const &project_id, project_path const *project_path, project_format const &project_format,
    player *player, file_module_loading_state_holder *state_holder, database *database, file_track *file_track,
    edge_holder *edge_holder, timeline_holder const *timeline_holder) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<file_module_loader>(
        uuid_generator::make_shared(), project_id, project_path, project_format, app_lifetime->file_importer.get(),
        app_lifetime->file_info_loader.get(), player, state_holder, database, file_track, edge_holder, timeline_holder);
}

file_module_loader::file_module_loader(std::shared_ptr<uuid_generatable> const &uuid_generator,
                                       project_id const &project_id, project_path const *project_path,
                                       project_format const &project_format, file_importer *file_importer,
                                       file_info_loader const *file_info_loader, player *player,
                                       file_module_loading_state_holder *state_holder, database *database,
                                       file_track *file_track, edge_holder *edge_holder,
                                       timeline_holder const *timeline_holder)
    : _uuid_generator(uuid_generator),
      _project_id(project_id),
      _project_path(project_path),
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

void file_module_loader::load(std::filesystem::path const &src_path) {
    if (this->_state_holder->state() != file_module_loading_state::waiting) {
        assertion_failure_if_not_test();
        return;
    }

    this->_state_holder->set_state(file_module_loading_state::loading);

    auto const uuid = this->_uuid_generator->generate();
    std::string const src_file_name = src_path.filename();
    std::string const dst_file_name = uuid + ".caf";
    auto const dst_path = this->_project_path->editing_files_directory().append(dst_file_name);

    this->_file_importer->import(
        {.project_id = this->_project_id,
         .src_path = src_path,
         .dst_path = dst_path,
         .project_format = this->_project_format,
         .completion = [weak = this->weak_from_this(), dst_path, src_file_name, dst_file_name](bool const result) {
             auto const loader = weak.lock();
             if (!loader) {
                 assertion_failure_if_not_test();
                 return;
             }

             if (auto const file_info = loader->_file_info_loader->load_file_info(dst_path)) {
                 loader->_database->suspend_saving(
                     [&loader, &file_info = file_info.value(), &src_file_name, &dst_file_name] {
                         loader->_file_track->overwrite_module(file_module{.name = src_file_name,
                                                                           .range = time::range{0, file_info.length},
                                                                           .file_frame = 0,
                                                                           .file_name = dst_file_name});

                         if (auto const &total_range = loader->_file_track->total_range()) {
                             auto const &total_range_value = total_range.value();
                             loader->_edge_holder->set_edge(
                                 {.begin_frame = total_range_value.frame,
                                  .end_frame = static_cast<frame_index_t>(total_range_value.next_frame())});
                         }
                     });
             }

             loader->_state_holder->set_state(file_module_loading_state::waiting);
         }});
}
