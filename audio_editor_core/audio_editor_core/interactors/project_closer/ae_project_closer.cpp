//
//  ae_project_closer.cpp
//

#include "ae_project_closer.h"

#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_status.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_closer> project_closer::make_shared(
    project_id const &project_id, file_importer_for_project_closer *file_importer,
    project_level_router_for_project_closer *project_level_router,
    project_editor_level_pool_for_project_closer *editor_level_pool, project_status_for_project_closer *status) {
    return std::make_shared<project_closer>(project_id, file_importer, project_level_router, editor_level_pool, status);
}

project_closer::project_closer(project_id const &project_id, file_importer_for_project_closer *file_importer,
                               project_level_router_for_project_closer *project_level_router,
                               project_editor_level_pool_for_project_closer *editor_level_pool,
                               project_status_for_project_closer *status)
    : _project_id(project_id),
      _file_importer(file_importer),
      _project_level_router(project_level_router),
      _editor_level_pool(editor_level_pool),
      _status(status) {
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
            this->_editor_level_pool->remove_level();
            this->_status->set_state(project_state::closing);
            break;
        case project_state::launching:
        case project_state::failure:
            this->_status->set_state(project_state::closing);
            break;
    }

    this->_project_level_router->remove_level(this->_project_id);
}
