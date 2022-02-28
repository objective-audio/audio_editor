//
//  ae_ui_root_level_pool.mm
//

#include "ae_ui_root_level_pool.h"
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level_pool> ui_root_level_pool::make_shared() {
    return std::shared_ptr<ui_root_level_pool>(new ui_root_level_pool{});
}

ui_root_level_pool::ui_root_level_pool() {
}

void ui_root_level_pool::add_level(std::shared_ptr<ui::standard> const &standard, std::string const &project_id,
                                   uintptr_t const project_view_id) {
    assert(!this->_ui_roots.contains(project_view_id));

    auto value = ui_root_level::make_shared(standard, project_id, project_view_id);
    this->_ui_roots.emplace(project_view_id, std::move(value));
}

std::shared_ptr<ui_root_level> const &ui_root_level_pool::level_for_view_id(uintptr_t const project_view_id) const {
    if (this->_ui_roots.contains(project_view_id)) {
        return this->_ui_roots.at(project_view_id);
    } else {
        static std::shared_ptr<ui_root_level> const _null_value = nullptr;
        return _null_value;
    }
}

void ui_root_level_pool::remove_level_for_view_id(uintptr_t const view_id) {
    if (this->_ui_roots.contains(view_id)) {
        this->_ui_roots.erase(view_id);
    }
}
