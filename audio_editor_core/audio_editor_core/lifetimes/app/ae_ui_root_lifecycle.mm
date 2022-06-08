//
//  ae_ui_root_lifecycle.mm
//

#include "ae_ui_root_lifecycle.h"
#include <audio_editor_core/ae_ui_root.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_lifecycle> ui_root_lifecycle::make_shared() {
    return std::make_shared<ui_root_lifecycle>();
}

ui_root_lifecycle::ui_root_lifecycle() {
}

void ui_root_lifecycle::add_lifetime(std::shared_ptr<ui::standard> const &standard, ui_project_id const &project_id) {
    assert(!this->_ui_roots.contains(project_id.view_id));

    auto const lifetime = ui_root_lifetime::make_shared(standard, project_id);
    this->_ui_roots.emplace(project_id.view_id, lifetime);
}

std::shared_ptr<ui_root_lifetime> const &ui_root_lifecycle::lifetime_for_view_id(
    uintptr_t const project_view_id) const {
    if (this->_ui_roots.contains(project_view_id)) {
        return this->_ui_roots.at(project_view_id);
    } else {
        static std::shared_ptr<ui_root_lifetime> const _null_value = nullptr;
        return _null_value;
    }
}

void ui_root_lifecycle::remove_lifetime_for_view_id(uintptr_t const view_id) {
    if (this->_ui_roots.contains(view_id)) {
        this->_ui_roots.erase(view_id);
    }
}
