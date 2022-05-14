//
//  ae_project_launcher.cpp
//

#include "ae_project_launcher.h"

#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_project_editor_responder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launcher> project_launcher::make_shared(
    project_id const &project_id, project_format const &project_format, url const &file_url,
    project_url_for_project_launcher const *project_url, file_importer_for_project_launcher *file_importer,
    file_info_loader_for_project_launcher const *file_info_loader,
    responder_stack_for_project_launcher *responder_stack,
    project_editor_level_pool_for_project_launcher *editor_level_pool,
    project_state_holder_for_project_launcher *state_holder) {
    return std::make_shared<ae::project_launcher>(project_id, project_format, file_url, project_url, file_importer,
                                                  file_info_loader, responder_stack, editor_level_pool, state_holder);
}

project_launcher::project_launcher(project_id const &project_id, project_format const &project_format,
                                   url const &file_url, project_url_for_project_launcher const *project_url,
                                   file_importer_for_project_launcher *file_importer,
                                   file_info_loader_for_project_launcher const *file_info_loader,
                                   responder_stack_for_project_launcher *responder_stack,
                                   project_editor_level_pool_for_project_launcher *editor_level_pool,
                                   project_state_holder_for_project_launcher *state_holder)
    : _project_id(project_id),
      _project_format(project_format),
      _file_url(file_url),
      _project_url(project_url),
      _file_importer(file_importer),
      _file_info_loader(file_info_loader),
      _responder_stack(responder_stack),
      _editor_level_pool(editor_level_pool),
      _state_holder(state_holder) {
}

void project_launcher::launch() {
    if (this->_state_holder->state() != project_state::launching) {
        assertion_failure_if_not_test();
        return;
    }

    auto const file_info = this->_file_info_loader->load_file_info(this->_file_url);
    if (!file_info.has_value()) {
        assertion_failure_if_not_test();
        this->_state_holder->set_state(project_state::failure);
        return;
    }

    this->_state_holder->set_state(project_state::loading);

    this->_editor_level_pool->add_level(file_info.value());

    auto const &level = this->_editor_level_pool->level();
    if (level) {
        this->_responder_stack->push_responder(level->instance_id, level->responder);
    }

    this->_state_holder->set_state(project_state::editing);
}
