//
//  ae_file_module_loader.cpp
//

#include "ae_file_module_loader.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_file_module_loading_state_holder.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_module_loader> file_module_loader::make_shared(project_id const &project_id,
                                                                    project_path const *project_path,
                                                                    project_format const &project_format,
                                                                    file_module_loading_state_holder *state_holder,
                                                                    database *database, pasteboard *pasteboard) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<file_module_loader>(uuid_generator::make_shared(), project_id, project_path, project_format,
                                                app_lifetime->file_importer.get(), app_lifetime->file_info_loader.get(),
                                                state_holder, database, pasteboard);
}

file_module_loader::file_module_loader(std::shared_ptr<uuid_generatable> const &uuid_generator,
                                       project_id const &project_id, project_path const *project_path,
                                       project_format const &project_format, file_importer *file_importer,
                                       file_info_loader const *file_info_loader,
                                       file_module_loading_state_holder *state_holder, database *database,
                                       pasteboard *pasteboard)
    : _uuid_generator(uuid_generator),
      _project_id(project_id),
      _project_path(project_path),
      _project_format(project_format),
      _file_importer(file_importer),
      _file_info_loader(file_info_loader),
      _state_holder(state_holder),
      _database(database),
      _pasteboard(pasteboard) {
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
                 loader->_pasteboard->set_file_modules({pasting_file_module_object{
                     identifier{},
                     pasting_file_module{src_file_name, 0, time::range{0, file_info.value().length}, dst_file_name}}});
             }

             loader->_state_holder->set_state(file_module_loading_state::waiting);
         }});
}
