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

void ui_root_lifecycle::add_lifetime(std::shared_ptr<ui::standard> const &standard,
                                     window_lifetime_id const &window_lifetime_id) {
    assert(!this->_ui_roots.contains(window_lifetime_id));

    auto const lifetime = ui_root_lifetime::make_shared(standard, window_lifetime_id);
    this->_ui_roots.emplace(window_lifetime_id, lifetime);
}

std::shared_ptr<ui_root_lifetime> const &ui_root_lifecycle::lifetime_for_window_lifetime_id(
    window_lifetime_id const &window_lifetime_id) const {
    if (this->_ui_roots.contains(window_lifetime_id)) {
        return this->_ui_roots.at(window_lifetime_id);
    } else {
        static std::shared_ptr<ui_root_lifetime> const _null_value = nullptr;
        return _null_value;
    }
}

void ui_root_lifecycle::remove_lifetime_for_window_lifetime_id(window_lifetime_id const &window_lifetime_id) {
    if (this->_ui_roots.contains(window_lifetime_id)) {
        this->_ui_roots.erase(window_lifetime_id);
    }
}
