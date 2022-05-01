//
//  ae_project_closer.cpp
//

#include "ae_project_closer.h"

#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_status.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_closer> project_closer::make_shared(
    std::string const &project_id, std::shared_ptr<file_importer_for_project_closer> const &file_importer,
    std::shared_ptr<project_level_pool_for_project_closer> const &project_level_pool,
    std::shared_ptr<project_editor_level_pool_for_project_closer> const &editor_level_pool,
    std::shared_ptr<project_status_for_project_closer> const &status) {
    return std::shared_ptr<project_closer>(
        new project_closer{project_id, file_importer, project_level_pool, editor_level_pool, status});
}

project_closer::project_closer(std::string const &project_id,
                               std::shared_ptr<file_importer_for_project_closer> const &file_importer,
                               std::shared_ptr<project_level_pool_for_project_closer> const &project_level_pool,
                               std::shared_ptr<project_editor_level_pool_for_project_closer> const &editor_level_pool,
                               std::shared_ptr<project_status_for_project_closer> const &status)
    : _project_id(project_id),
      _file_importer(file_importer),
      _project_level_pool(project_level_pool),
      _editor_level_pool(editor_level_pool),
      _status(status),
      _event_notifier(observing::notifier<project_event>::make_shared()) {
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

    if (auto const pool = this->_project_level_pool.lock()) {
        pool->remove_level(this->_project_id);
    }
}

observing::endable project_closer::observe_event(std::function<void(project_event const &)> &&handler) {
    return this->_event_notifier->observe(std::move(handler));
}
