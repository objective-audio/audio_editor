//
//  ae_ui_root_level_router.mm
//

#include "ae_ui_root_level_router.h"
#include <audio_editor_core/ae_ui_root.h>
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level_router> ui_root_level_router::make_shared() {
    return std::make_shared<ui_root_level_router>();
}

ui_root_level_router::ui_root_level_router() {
}

void ui_root_level_router::add_level(std::shared_ptr<ui::standard> const &standard, ui_project_id const &project_id) {
    assert(!this->_ui_roots.contains(project_id.view_id));

    auto const level = ui_root_level::make_shared(standard, project_id);
    this->_ui_roots.emplace(project_id.view_id, level);
    level->root->setup();
}

std::shared_ptr<ui_root_level> const &ui_root_level_router::level_for_view_id(uintptr_t const project_view_id) const {
    if (this->_ui_roots.contains(project_view_id)) {
        return this->_ui_roots.at(project_view_id);
    } else {
        static std::shared_ptr<ui_root_level> const _null_value = nullptr;
        return _null_value;
    }
}

void ui_root_level_router::remove_level_for_view_id(uintptr_t const view_id) {
    if (this->_ui_roots.contains(view_id)) {
        this->_ui_roots.erase(view_id);
    }
}
