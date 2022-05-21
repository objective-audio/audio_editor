//
//  ae_project_sub_level_router.cpp
//

#include "ae_project_sub_level_router.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<time_editor_level> const _null_time_editor_level = nullptr;
static std::shared_ptr<sheet_level> const _null_sheet_level = nullptr;

std::shared_ptr<project_sub_level_router> project_sub_level_router::make_shared(project_id const &project_id) {
    return std::make_shared<project_sub_level_router>(project_id);
}

project_sub_level_router::project_sub_level_router(project_id const &project_id)
    : _project_id(project_id),
      _sub_level(observing::value::holder<std::optional<project_sub_level>>::make_shared(std::nullopt)) {
}

std::optional<project_sub_level> const &project_sub_level_router::sub_level() const {
    return this->_sub_level->value();
}

void project_sub_level_router::add_time_editor(number_components const &components,
                                               std::optional<std::size_t> const unit_idx) {
    if (this->_sub_level->value().has_value()) {
        throw std::runtime_error("sub level is not null.");
    }

    this->_sub_level->set_value(time_editor_level::make_shared(this->_project_id, components, unit_idx));
}

void project_sub_level_router::remove_time_editor() {
    if (this->time_editor_level() == nullptr) {
        throw std::runtime_error("time_editor_level is null.");
    }

    this->_sub_level->set_value(std::nullopt);
}

std::shared_ptr<time_editor_level> const &project_sub_level_router::time_editor_level() const {
    return get_level<ae::time_editor_level>(this->_sub_level->value());
}

void project_sub_level_router::add_sheet(sheet_content const &content) {
    if (this->_sub_level->value().has_value()) {
        throw std::runtime_error("sub level is not null.");
    }

    this->_sub_level->set_value(sheet_level::make_shared(content));
}

void project_sub_level_router::remove_sheet() {
    if (this->sheet_level() == nullptr) {
        throw std::runtime_error("sheet_level is null.");
    }

    this->_sub_level->set_value(std::nullopt);
}

std::shared_ptr<sheet_level> const &project_sub_level_router::sheet_level() const {
    return get_level<ae::sheet_level>(this->_sub_level->value());
}

void project_sub_level_router::add_dialog(dialog_content const content) {
    if (this->_sub_level->value().has_value()) {
        throw std::runtime_error("sub level is not null.");
    }

    this->_sub_level->set_value(dialog_level::make_shared(content));
}

void project_sub_level_router::remove_dialog() {
    if (this->dialog_level() == nullptr) {
        throw std::runtime_error("sheet_level is null.");
    }

    this->_sub_level->set_value(std::nullopt);
}

std::shared_ptr<dialog_level> const &project_sub_level_router::dialog_level() const {
    return get_level<ae::dialog_level>(this->_sub_level->value());
}

void project_sub_level_router::add_context_menu(context_menu const &context_menu) {
    if (this->_sub_level->value().has_value()) {
        throw std::runtime_error("sub level is not null.");
    }

    this->_sub_level->set_value(context_menu_level::make_shared(context_menu));
}

void project_sub_level_router::remove_context_menu() {
    if (this->context_menu_level() == nullptr) {
        throw std::runtime_error("sheet_level is null.");
    }

    this->_sub_level->set_value(std::nullopt);
}

std::shared_ptr<context_menu_level> const &project_sub_level_router::context_menu_level() const {
    return get_level<ae::context_menu_level>(this->_sub_level->value());
}

observing::syncable project_sub_level_router::observe(
    std::function<void(std::optional<project_sub_level> const &)> &&handler) {
    return this->_sub_level->observe(std::move(handler));
}
