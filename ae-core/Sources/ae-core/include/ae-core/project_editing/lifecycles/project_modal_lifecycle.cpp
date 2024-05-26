//
//  project_modal_lifecycle.cpp
//

#include "project_modal_lifecycle.h"

#include <ae-core/app/features/id_generator.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/time_editor/features/time_editor.h>
#include <ae-core/time_editor/features/time_editor_receiver.h>

#include <ae-core/marker_name_sheet/features/marker_name_editor.hpp>
#include <ae-core/module_name_sheet/features/module_name_editor.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_modal_lifecycle> project_modal_lifecycle::make_shared(
    project_lifetime_id const &project_lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    return std::make_shared<project_modal_lifecycle>(app_lifetime->id_generator.get(), project_lifetime_id);
}

project_modal_lifecycle::project_modal_lifecycle(id_generatable const *id_generator,
                                                 project_lifetime_id const &project_lifetime_id)
    : _id_generator(id_generator),
      _project_lifetime_id(project_lifetime_id),
      _current(observing::value::holder<std::optional<project_modal_sub_lifetime>>::make_shared(std::nullopt)) {
}

std::optional<project_modal_sub_lifetime> const &project_modal_lifecycle::current() const {
    return this->_current->value();
}

void project_modal_lifecycle::add_time_editor(number_components const &components) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(time_editing_lifetime::make_shared(
        {.instance = this->_id_generator->generate(), .project = this->_project_lifetime_id}, components));
}

void project_modal_lifecycle::remove_time_editor(project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->time_editing_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("time_editor is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("time_editor does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<time_editing_lifetime> const &project_modal_lifecycle::time_editing_lifetime() const {
    return get<ae::time_editing_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_module_name_sheet(module_index const &index) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(std::make_shared<ae::module_name_sheet_lifetime>(
        project_sub_lifetime_id{.instance = this->_id_generator->generate(), .project = this->_project_lifetime_id},
        index));
}

void project_modal_lifecycle::remove_module_name_sheet(project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->module_name_sheet_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("module_name_sheet is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("sheet does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<module_name_sheet_lifetime> const &project_modal_lifecycle::module_name_sheet_lifetime() const {
    return get<ae::module_name_sheet_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_marker_name_sheet(marker_index const &index) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(std::make_shared<ae::marker_name_sheet_lifetime>(
        project_sub_lifetime_id{.instance = this->_id_generator->generate(), .project = this->_project_lifetime_id},
        index));
}

void project_modal_lifecycle::remove_marker_name_sheet(project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->marker_name_sheet_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("module_name_sheet is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("sheet does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<marker_name_sheet_lifetime> const &project_modal_lifecycle::marker_name_sheet_lifetime() const {
    return get<ae::marker_name_sheet_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_file_import_dialog() {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(std::make_shared<ae::file_import_dialog_lifetime>(
        project_sub_lifetime_id{.instance = this->_id_generator->generate(), .project = this->_project_lifetime_id}));
}

void project_modal_lifecycle::remove_file_import_dialog(project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->file_import_dialog_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("file_import_dialog is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("file_import_dialog does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<file_import_dialog_lifetime> const &project_modal_lifecycle::file_import_dialog_lifetime() const {
    return get<ae::file_import_dialog_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_file_export_dialog() {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(std::make_shared<ae::file_export_dialog_lifetime>(
        project_sub_lifetime_id{.instance = this->_id_generator->generate(), .project = this->_project_lifetime_id}));
}

void project_modal_lifecycle::remove_file_export_dialog(project_sub_lifetime_id const &lifetime_id) {
    auto const &lifetime = this->file_export_dialog_lifetime();

    if (lifetime == nullptr) {
        throw std::runtime_error("file_export_dialog is null.");
    } else if (lifetime->lifetime_id != lifetime_id) {
        throw std::runtime_error("file_export_dialog does not match id.");
    }

    this->_current->set_value(std::nullopt);
}

std::shared_ptr<file_export_dialog_lifetime> const &project_modal_lifecycle::file_export_dialog_lifetime() const {
    return get<ae::file_export_dialog_lifetime>(this->_current->value());
}

void project_modal_lifecycle::add_context_menu(context_menu const &context_menu) {
    if (this->_current->value().has_value()) {
        throw std::runtime_error("current is not null.");
    }

    this->_current->set_value(std::make_shared<ae::context_menu_lifetime>(
        project_sub_lifetime_id{.instance = this->_id_generator->generate(), .project = this->_project_lifetime_id},
        context_menu));
}

void project_modal_lifecycle::remove_context_menu(project_sub_lifetime_id const &lifetime_id) {
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
    return action_id{this->_project_lifetime_id};
}

std::vector<action_receivable *> project_modal_lifecycle::receivers() const {
    if (auto const &lifetime = get<ae::time_editing_lifetime>(this->current())) {
        return {lifetime->receiver.get()};
    } else {
        return {};
    }
}

std::vector<action_receiver_providable *> project_modal_lifecycle::sub_providers() const {
    return {};
}
