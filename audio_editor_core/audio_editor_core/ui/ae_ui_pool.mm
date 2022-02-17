//
//  ae_ui_pool.mm
//

#include "ae_ui_pool.h"
#include <audio_editor_core/ae_ui_root.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_pool> ui_pool::make_shared() {
    return std::shared_ptr<ui_pool>(new ui_pool{});
}

ui_pool::ui_pool() {
}

std::shared_ptr<ui_root> const &ui_pool::add_and_return_ui_root(std::shared_ptr<ui::standard> const &standard,
                                                                std::string const &project_id,
                                                                uintptr_t const project_view_id) {
    assert(!this->_ui_roots.contains(project_view_id));

    auto value = ui_root::make_shared(standard, project_id, project_view_id);
    this->_ui_roots.emplace(project_view_id, std::move(value));
    return this->_ui_roots.at(project_view_id);
}

std::shared_ptr<ui_root> const &ui_pool::ui_root_for_view_id(uintptr_t const project_view_id) const {
    if (this->_ui_roots.contains(project_view_id)) {
        return this->_ui_roots.at(project_view_id);
    } else {
        static std::shared_ptr<ui_root> const _null_value = nullptr;
        return _null_value;
    }
}

void ui_pool::remove_ui_root_for_view_id(uintptr_t const view_id) {
    if (this->_ui_roots.contains(view_id)) {
        this->_ui_roots.erase(view_id);
    }
}
