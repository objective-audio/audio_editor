//
//  ae_project_editor_level_pool.cpp
//

#include "ae_project_editor_level_pool.h"

#include <audio_editor_core/ae_project_editor_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level_pool> project_editor_level_pool::make_shared(std::string const &identifier) {
    return std::shared_ptr<project_editor_level_pool>(new project_editor_level_pool{identifier});
}

project_editor_level_pool::project_editor_level_pool(std::string const &identifier)
    : _identifier(identifier), _editor_level(nullptr) {
}

void project_editor_level_pool::add_level(file_info const &file_info) {
    if (this->_editor_level != nullptr) {
        throw std::runtime_error("editor_level is not null.");
    }

    this->_editor_level = project_editor_level::make_shared(this->_identifier, file_info);
}

void project_editor_level_pool::remove_level() {
    if (this->_editor_level == nullptr) {
        throw std::runtime_error("editor_level is null.");
    }

    this->_editor_level = nullptr;
}

std::shared_ptr<project_editor_level> const &project_editor_level_pool::level() const {
    return this->_editor_level;
}
