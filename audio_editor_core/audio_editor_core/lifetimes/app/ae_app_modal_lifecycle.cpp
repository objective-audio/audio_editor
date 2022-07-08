//
//  ae_app_modal_lifecycle.cpp
//

#include "ae_app_modal_lifecycle.h"

#include <audio_editor_core/ae_action_id.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_modal_lifecycle> app_modal_lifecycle::make_shared() {
    return std::make_shared<app_modal_lifecycle>();
}

app_modal_lifecycle::app_modal_lifecycle()
    : _current(observing::value::holder<std::optional<app_modal_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<app_modal_sub_lifetime> const &app_modal_lifecycle::current() const {
    return this->_current->value();
}

void app_modal_lifecycle::add_project_setup_dialog() {
    if (this->_current->value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(project_setup_dialog_lifetime::make_shared({.instance = identifier{}}));
}

void app_modal_lifecycle::remove_project_setup_dialog() {
    if (this->project_setup_dialog_lifetime() == nullptr) {
        throw std::runtime_error("project_setup_dialog_lifetime is null.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<project_setup_dialog_lifetime> const &app_modal_lifecycle::project_setup_dialog_lifetime() const {
    return get<ae::project_setup_dialog_lifetime>(this->_current->value());
}

observing::syncable app_modal_lifecycle::observe(
    std::function<void(std::optional<app_modal_sub_lifetime> const &)> &&handler) {
    return this->_current->observe(
        [handler = std::move(handler)](std::optional<app_modal_sub_lifetime> const &sub_lifetime) {
            handler(sub_lifetime);
        });
}

#pragma mark - action_receiver_provider

std::optional<action_id> app_modal_lifecycle::receivable_id() const {
    return std::nullopt;
}

std::vector<action_receivable *> app_modal_lifecycle::receivers() const {
    return {};
}

std::vector<action_receiver_providable *> app_modal_lifecycle::sub_providers() const {
    return {};
}
