//
//  ae_project_launcher.cpp
//

#include "ae_project_launcher.h"

#include <audio_editor_core/ae_project_editor_responder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_launcher> project_launcher::make_shared(
    identifier const &instance_id, url const &file_url, file_info_loader_for_project_launcher const *file_info_loader,
    responder_stack_for_project_launcher *responder_stack, project_state_holder_for_project_launcher *state_holder,
    std::shared_ptr<project_editor_responder> const &responder) {
    return std::make_shared<ae::project_launcher>(instance_id, file_url, file_info_loader, responder_stack,
                                                  state_holder, responder);
}

project_launcher::project_launcher(identifier const &instance_id, url const &file_url,
                                   file_info_loader_for_project_launcher const *file_info_loader,
                                   responder_stack_for_project_launcher *responder_stack,
                                   project_state_holder_for_project_launcher *state_holder,
                                   std::shared_ptr<project_editor_responder> const &responder)
    : _instance_id(instance_id),
      _file_url(file_url),
      _file_info_loader(file_info_loader),
      _responder_stack(responder_stack),
      _state_holder(state_holder),
      _responder(responder) {
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

    auto const responder = this->_responder.lock();
    if (!responder) {
        assertion_failure_if_not_test();
        return;
    }

    this->_state_holder->set_state(project_state::loading);

    this->_responder_stack->push_responder(this->_instance_id, responder);

    this->_state_holder->set_state(project_state::editing);
}
