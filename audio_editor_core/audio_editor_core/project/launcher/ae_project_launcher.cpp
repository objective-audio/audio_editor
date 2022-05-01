//
//  ae_project_launcher.cpp
//

#include "ae_project_launcher.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_editor_responder.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launcher> project_launcher::make_shared(
    std::string const &project_id, url const &file_url,
    std::shared_ptr<project_url_for_project_launcher> const &project_url,
    std::shared_ptr<file_importer_for_project_launcher> const &file_importer,
    std::shared_ptr<file_loader_for_project_launcher> const &file_loader,
    std::shared_ptr<responder_stack_for_project_launcher> const &responder_stack,
    std::shared_ptr<project_editor_level_pool_for_project_launcher> const &editor_level_pool,
    std::shared_ptr<project_status_for_project_launcher> const &status) {
    auto launcher = std::shared_ptr<ae::project_launcher>(new ae::project_launcher{
        project_id, file_url, project_url, file_importer, file_loader, responder_stack, editor_level_pool, status});
    launcher->_weak_launcher = launcher;
    return launcher;
}

project_launcher::project_launcher(
    std::string const &project_id, url const &file_url,
    std::shared_ptr<project_url_for_project_launcher> const &project_url,
    std::shared_ptr<file_importer_for_project_launcher> const &file_importer,
    std::shared_ptr<file_loader_for_project_launcher> const &file_loader,
    std::shared_ptr<responder_stack_for_project_launcher> const &responder_stack,
    std::shared_ptr<project_editor_level_pool_for_project_launcher> const &editor_level_pool,
    std::shared_ptr<project_status_for_project_launcher> const &status)
    : _project_id(project_id),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_loader(file_loader),
      _responder_stack(responder_stack),
      _editor_level_pool(editor_level_pool),
      _status(status) {
}

void project_launcher::launch() {
    this->_status->set_state(project_state::loading);

    this->_file_importer->import(
        {.identifier = this->_project_id,
         .src_url = this->_file_url,
         .dst_url = this->_project_url->editing_file(),
         .completion = [weak = this->_weak_launcher](bool const result) {
             if (auto const launcher = weak.lock()) {
                 auto const &state = launcher->_status->state();
                 switch (state) {
                     case project_state::loading: {
                         if (result) {
                             auto const editing_file_url = launcher->_project_url->editing_file();
                             if (auto const file_info = launcher->_file_loader->load_file_info(editing_file_url)) {
                                 launcher->_editor_level_pool->add_level(file_info.value());

                                 auto const responder_stack = launcher->_responder_stack.lock();
                                 auto const level = launcher->_editor_level_pool->level();
                                 if (responder_stack && level) {
                                     responder_stack->push_responder(level->instance_id, level->responder);
                                 }

                                 launcher->_status->set_state(project_state::editing);
                             } else {
                                 launcher->_status->set_state(project_state::failure);
                             }
                         } else {
                             launcher->_status->set_state(project_state::failure);
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
