//
//  ae_project_editor_level_pool.cpp
//

#include "ae_project_editor_level_pool.h"

#include <audio_editor_core/ae_project_editor_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level_pool> project_editor_level_pool::make_shared(project_id const &project_id) {
    return std::shared_ptr<project_editor_level_pool>(new project_editor_level_pool{project_id});
}

project_editor_level_pool::project_editor_level_pool(project_id const &project_id)
    : _project_id(project_id),
      _editor_level(observing::value::holder<std::shared_ptr<project_editor_level>>::make_shared(nullptr)) {
}

void project_editor_level_pool::add_level(file_info const &file_info) {
    if (this->_editor_level->value() != nullptr) {
        throw std::runtime_error("editor_level is not null.");
    }

    this->_editor_level->set_value(project_editor_level::make_shared(this->_project_id, file_info));
}

void project_editor_level_pool::remove_level() {
    if (this->_editor_level->value() == nullptr) {
        throw std::runtime_error("editor_level is null.");
    }

    this->_editor_level->set_value(nullptr);
}

std::shared_ptr<project_editor_level> const &project_editor_level_pool::level() const {
    return this->_editor_level->value();
}

observing::syncable project_editor_level_pool::observe_level(
    std::function<void(std::shared_ptr<project_editor_level> const &)> &&handler) {
    return this->_editor_level->observe(std::move(handler));
}
