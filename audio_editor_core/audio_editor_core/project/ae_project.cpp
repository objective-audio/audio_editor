//
//  ae_project.cpp
//

#include "ae_project.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_editor_responder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project> project::make_shared(
    std::string const &identifier, url const &file_url, std::shared_ptr<project_url_for_project> const &project_url,
    std::shared_ptr<file_importer_for_project> const &file_importer,
    std::shared_ptr<file_loader_for_project> const &file_loader,
    std::shared_ptr<responder_stack_for_project> const &responder_stack,
    std::shared_ptr<project_editor_level_pool_for_project> const &editor_level_pool,
    std::shared_ptr<project_status_for_project> const &status) {
    auto project = std::shared_ptr<ae::project>(new ae::project{
        identifier, file_url, project_url, file_importer, file_loader, responder_stack, editor_level_pool, status});
    project->_weak_project = project;
    return project;
}

project::project(std::string const &identifier, url const &file_url,
                 std::shared_ptr<project_url_for_project> const &project_url,
                 std::shared_ptr<file_importer_for_project> const &file_importer,
                 std::shared_ptr<file_loader_for_project> const &file_loader,
                 std::shared_ptr<responder_stack_for_project> const &responder_stack,
                 std::shared_ptr<project_editor_level_pool_for_project> const &editor_level_pool,
                 std::shared_ptr<project_status_for_project> const &status)
    : _identifier(identifier),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_loader(file_loader),
      _responder_stack(responder_stack),
      _editor_level_pool(editor_level_pool),
      _status(status) {
}

void project::setup() {
    this->_status->set_state(project_state::loading);

    this->_file_importer->import(
        {.identifier = this->_identifier,
         .src_url = this->_file_url,
         .dst_url = this->_project_url->editing_file(),
         .completion = [weak = this->_weak_project](bool const result) {
             if (auto const project = weak.lock()) {
                 auto const &state = project->_status->state();
                 switch (state) {
                     case project_state::loading: {
                         if (result) {
                             auto const editing_file_url = project->_project_url->editing_file();
                             if (auto const file_info = project->_file_loader->load_file_info(editing_file_url)) {
                                 project->_editor_level_pool->add_level(file_info.value());

                                 auto const responder_stack = project->_responder_stack.lock();
                                 auto const level = project->_editor_level_pool->level();
                                 if (responder_stack && level) {
                                     responder_stack->push_responder(level->instance_id, level->responder);
                                 }

                                 project->_status->set_state(project_state::editing);
                             } else {
                                 project->_status->set_state(project_state::failure);
                             }
                         } else {
                             project->_status->set_state(project_state::failure);
                         }
                     } break;

                     case project_state::launching:
                     case project_state::editing:
                     case project_state::failure:
                     case project_state::closing:
                         break;
                 }
             }
         }});
}
