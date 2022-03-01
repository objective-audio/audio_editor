//
//  ae_ui_editing_root_level_pool.cpp
//

#include "ae_ui_editing_root_level_pool.h"
#include <audio_editor_core/ae_ui_editing_root_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_editing_root_level_pool> ui_editing_root_level_pool::make_shared(
    std::string const &project_id, std::uintptr_t const project_view_id) {
    return std::shared_ptr<ui_editing_root_level_pool>(new ui_editing_root_level_pool{project_id, project_view_id});
}

ui_editing_root_level_pool::ui_editing_root_level_pool(std::string const &project_id,
                                                       std::uintptr_t const project_view_id)
    : _project_id(project_id), _project_view_id(project_view_id) {
}

void ui_editing_root_level_pool::add_level() {
    if (this->_level != nullptr) {
        throw std::runtime_error("level is not null.");
    }

    this->_level = ui_editing_root_level::make_shared(this->_project_id, this->_project_view_id);
}

std::shared_ptr<ui_editing_root_level> const &ui_editing_root_level_pool::level() const {
    return this->_level;
}
