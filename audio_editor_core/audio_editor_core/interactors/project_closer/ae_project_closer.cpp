//
//  ae_project_closer.cpp
//

#include "ae_project_closer.h"

#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_project_state_holder.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_closer> project_closer::make_shared(project_id const &project_id,
                                                            file_importer_for_project_closer *file_importer,
                                                            window_lifecycle_for_project_closer *window_lifecycle,
                                                            project_state_holder_for_project_closer *state_holder) {
    return std::make_shared<project_closer>(project_id, file_importer, window_lifecycle, state_holder);
}

project_closer::project_closer(project_id const &project_id, file_importer_for_project_closer *file_importer,
                               window_lifecycle_for_project_closer *window_lifecycle,
                               project_state_holder_for_project_closer *state_holder)
    : _project_id(project_id),
      _file_importer(file_importer),
      _window_lifecycle(window_lifecycle),
      _status(state_holder) {
}

bool project_closer::can_close() const {
    return true;
}

void project_closer::request_close() {
    switch (this->_status->state()) {
        case project_state::closing:
            return;
        case project_state::loading:
            this->_file_importer->cancel(this->_project_id);
            this->_status->set_state(project_state::closing);
            break;
        case project_state::editing:
            this->_status->set_state(project_state::closing);
            break;
        case project_state::launching:
        case project_state::failure:
            this->_status->set_state(project_state::closing);
            break;
    }

    this->_window_lifecycle->remove_lifetime(this->_project_id);
}
