//
//  ae_app_settings_lifecycle.cpp
//

#include "ae_app_settings_lifecycle.hpp"

#include <audio_editor_core/ae_app_settings_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<app_settings_lifetime> const _empty_lifetime = nullptr;

app_settings_lifecycle::app_settings_lifecycle()
    : _current(observing::value::holder<std::shared_ptr<app_settings_lifetime>>::make_shared(nullptr)) {
}

void app_settings_lifecycle::add_lifetime() {
    if (this->has_current()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(app_settings_lifetime::make_shared(app_settings_lifetime_id{}));
}

void app_settings_lifecycle::remove_lifetime(app_settings_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->current();

    if (lifetime == nullptr) {
        throw std::runtime_error("project_setup_dialog_lifetime is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("project_setup_dialog_lifetime does not match id.");
    }

    this->_current->set_value(nullptr);
}

std::shared_ptr<app_settings_lifetime> const &app_settings_lifecycle::current() const {
    return this->_current->value();
}

bool app_settings_lifecycle::has_current() const {
    return this->_current->value() != nullptr;
}

observing::syncable app_settings_lifecycle::observe_event(
    std::function<void(app_settings_lifecycle_event const &)> &&handler) {
    auto prev_lifetime = std::shared_ptr<app_settings_lifetime>{};

    return this->_current->observe([handler = std::move(handler), prev_lifetime = std::move(prev_lifetime)](
                                       std::shared_ptr<app_settings_lifetime> const &lifetime) mutable {
        if (lifetime && !prev_lifetime) {
            handler(app_settings_lifecycle_event{.type = app_settings_lifecycle_event_type::inserted,
                                                 .lifetime_id = lifetime->lifetime_id});
        } else if (!lifetime && prev_lifetime) {
            handler(app_settings_lifecycle_event{.type = app_settings_lifecycle_event_type::erased,
                                                 .lifetime_id = prev_lifetime->lifetime_id});
        }

        prev_lifetime = lifetime;
    });
}
