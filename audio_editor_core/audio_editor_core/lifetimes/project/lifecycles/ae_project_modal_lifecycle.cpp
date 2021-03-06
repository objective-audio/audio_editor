//
//  ae_project_modal_lifecycle.cpp
//

#include "ae_project_modal_lifecycle.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_id_generator.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_lifetime.h>
#include <audio_editor_core/ae_time_editor_receiver.h>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<time_editor_lifetime> const _null_time_editor_lifetime = nullptr;
static std::shared_ptr<sheet_lifetime> const _null_sheet_lifetime = nullptr;

std::shared_ptr<project_modal_lifecycle> project_modal_lifecycle::make_shared(
    window_lifetime_id const &window_lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<project_modal_lifecycle>(app_lifetime->id_generator.get(), window_lifetime_id);
}

project_modal_lifecycle::project_modal_lifecycle(id_generatable const *id_generator,
                                                 window_lifetime_id const &window_lifetime_id)
    : _id_generator(id_generator),
      _window_lifetime_id(window_lifetime_id),
      _current(observing::value::holder<std::optional<project_modal_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<project_modal_sub_lifetime> const &project_modal_lifecycle::current() const {
    return this->_current->value();
}

void project_modal_lifecycle::add_time_editor(number_components const &components) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(time_editor_lifetime::make_shared(
        {.instance = this->_id_generator->generate(), .window = this->_window_lifetime_id}, components));
}

void project_modal_lifecycle::remove_time_editor(time_editor_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->time_editor_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("time_editor is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("time_editor does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<time_editor_lifetime> const &project_modal_lifecycle::time_editor_lifetime() const {
    return get<ae::time_editor_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_sheet(sheet_content const &content) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(sheet_lifetime::make_shared(
        {.instance = this->_id_generator->generate(), .window = this->_window_lifetime_id}, content));
}

void project_modal_lifecycle::remove_sheet(sheet_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->sheet_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("sheet is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("sheet does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<sheet_lifetime> const &project_modal_lifecycle::sheet_lifetime() const {
    return get<ae::sheet_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_dialog(dialog_content const content) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(dialog_lifetime::make_shared(
        {.instance = this->_id_generator->generate(), .window = this->_window_lifetime_id}, content));
}

void project_modal_lifecycle::remove_dialog(dialog_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->dialog_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("dialog is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("dialog does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<dialog_lifetime> const &project_modal_lifecycle::dialog_lifetime() const {
    return get<ae::dialog_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_context_menu(context_menu const &context_menu) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(context_menu_lifetime::make_shared(
        {.instance = this->_id_generator->generate(), .window = this->_window_lifetime_id}, context_menu));
}

void project_modal_lifecycle::remove_context_menu(context_menu_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->context_menu_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("context_menu is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("context_menu does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<context_menu_lifetime> const &project_modal_lifecycle::context_menu_lifetime() const {
    return get<ae::context_menu_lifetime>(this->_current->value());
}

observing::syncable project_modal_lifecycle::observe(
    std::function<void(std::optional<project_modal_sub_lifetime> const &)> &&handler) {
    return this->_current->observe(std::move(handler));
}

#pragma mark - action_receiver_provider

std::optional<action_id> project_modal_lifecycle::receivable_id() const {
    return action_id{this->_window_lifetime_id};
}

std::vector<action_receivable *> project_modal_lifecycle::receivers() const {
    if (auto const &lifetime = get<ae::time_editor_lifetime>(this->current())) {
        return {lifetime->receiver.get()};
    } else {
        return {};
    }
}

std::vector<action_receiver_providable *> project_modal_lifecycle::sub_providers() const {
    return {};
}
