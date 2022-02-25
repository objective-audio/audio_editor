//
//  ae_project_editor_level_pool.cpp
//

#include "ae_project_editor_level_pool.h"

#include <audio_editor_core/ae_project_editor_level_maker.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level_pool> project_editor_level_pool::make_shared(
    std::shared_ptr<project_editor_level_maker> const &maker) {
    return std::shared_ptr<project_editor_level_pool>(new project_editor_level_pool{maker});
}

project_editor_level_pool::project_editor_level_pool(std::shared_ptr<project_editor_level_maker> const &maker)
    : _maker(maker), _editor_level(nullptr) {
}

void project_editor_level_pool::add_editor_level(file_info const &file_info) {
    if (this->_editor_level != nullptr) {
        throw std::runtime_error("editor_level is not null.");
    }

    this->_editor_level = this->_maker->make(file_info);
}

std::shared_ptr<project_editor_level> const &project_editor_level_pool::editor_level() const {
    return this->_editor_level;
}
