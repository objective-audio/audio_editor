//
//  ui_resource_lifecycle.cpp
//

#include "ui_resource_lifecycle.hpp"

#include <ae-core/ui_resource/lifecycles/ui_base_lifecycle.hpp>
#include <ae-core/ui_resource/ui_resource_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

ui_resource_lifecycle::ui_resource_lifecycle() {
}

void ui_resource_lifecycle::add_lifetime(std::shared_ptr<ui::standard> const &standard,
                                         project_lifetime_id const &lifetime_id) {
    assert(!this->_lifetimes.contains(lifetime_id));

    auto const lifetime = std::make_shared<ui_resource_lifetime>(standard, lifetime_id);
    this->_lifetimes.emplace(lifetime_id, lifetime);
    lifetime->base_lifecycle->add_lifetime();
}

void ui_resource_lifecycle::remove_lifetime_for_project_lifetime_id(project_lifetime_id const &lifetime_id) {
    if (this->_lifetimes.contains(lifetime_id)) {
        this->_lifetimes.erase(lifetime_id);
    }
}

std::shared_ptr<ui_resource_lifetime> const &ui_resource_lifecycle::lifetime_for_project_lifetime_id(
    project_lifetime_id const &lifetime_id) const {
    if (this->_lifetimes.contains(lifetime_id)) {
        return this->_lifetimes.at(lifetime_id);
    } else {
        static std::shared_ptr<ui_resource_lifetime> const _null_value = nullptr;
        return _null_value;
    }
}
