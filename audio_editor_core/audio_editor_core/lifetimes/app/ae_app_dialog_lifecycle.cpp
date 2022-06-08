//
//  ae_app_dialog_lifecycle.cpp
//

#include "ae_app_dialog_lifecycle.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_dialog_lifecycle> app_dialog_lifecycle::make_shared() {
    return std::make_shared<app_dialog_lifecycle>();
}

app_dialog_lifecycle::app_dialog_lifecycle()
    : _current(observing::value::holder<std::optional<app_dialog_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<app_dialog_sub_lifetime> const &app_dialog_lifecycle::current() const {
    return this->_current->value();
}

void app_dialog_lifecycle::add_project_format_dialog() {
    if (this->_current->value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(project_setup_dialog_lifetime::make_shared());
}

void app_dialog_lifecycle::remove_project_format_dialog() {
    if (this->project_setup_dialog_lifetime() == nullptr) {
        throw std::runtime_error("project_setup_dialog_lifetime is null.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<project_setup_dialog_lifetime> const &app_dialog_lifecycle::project_setup_dialog_lifetime() const {
    return get<ae::project_setup_dialog_lifetime>(this->_current->value());
}

observing::syncable app_dialog_lifecycle::observe(
    std::function<void(std::optional<app_dialog_sub_lifetime> const &)> &&handler) {
    return this->_current->observe(
        [handler = std::move(handler)](std::optional<app_dialog_sub_lifetime> const &sub_lifetime) {
            handler(sub_lifetime);
        });
}
