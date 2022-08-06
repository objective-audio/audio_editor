//
//  ae_ui_resource_lifecycle.cpp
//

#include "ae_ui_resource_lifecycle.hpp"

#include <audio_editor_core/ae_ui_base_lifecycle.hpp>
#include <audio_editor_core/ae_ui_resource_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

ui_resource_lifecycle::ui_resource_lifecycle() {
}

void ui_resource_lifecycle::add_lifetime(std::shared_ptr<ui::standard> const &standard,
                                         window_lifetime_id const &window_lifetime_id) {
    assert(!this->_lifetimes.contains(window_lifetime_id));

    auto const lifetime = std::make_shared<ui_resource_lifetime>(standard, window_lifetime_id);
    this->_lifetimes.emplace(window_lifetime_id, lifetime);
    lifetime->base_lifecycle->add_lifetime();
}

void ui_resource_lifecycle::remove_lifetime_for_window_lifetime_id(window_lifetime_id const &window_lifetime_id) {
    if (this->_lifetimes.contains(window_lifetime_id)) {
        this->_lifetimes.erase(window_lifetime_id);
    }
}

std::shared_ptr<ui_resource_lifetime> const &ui_resource_lifecycle::lifetime_for_window_lifetime_id(
    window_lifetime_id const &window_lifetime_id) const {
    if (this->_lifetimes.contains(window_lifetime_id)) {
        return this->_lifetimes.at(window_lifetime_id);
    } else {
        static std::shared_ptr<ui_resource_lifetime> const _null_value = nullptr;
        return _null_value;
    }
}
