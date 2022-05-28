//
//  ae_app_dialog_sub_level_router.cpp
//

#include "ae_app_dialog_sub_level_router.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_dialog_sub_level_router> app_dialog_sub_level_router::make_shared() {
    return std::make_shared<app_dialog_sub_level_router>();
}

app_dialog_sub_level_router::app_dialog_sub_level_router()
    : _level(observing::value::holder<std::optional<app_dialog_sub_level>>::make_shared(std::nullopt)) {
}

std::optional<app_dialog_sub_level> const &app_dialog_sub_level_router::sub_level() const {
    return this->_level->value();
}

void app_dialog_sub_level_router::add_project_format_dialog() {
    if (this->_level->value()) {
        throw std::runtime_error("level is not null.");
    }

    this->_level->set_value(project_setup_dialog_level::make_shared());
}

void app_dialog_sub_level_router::remove_project_format_dialog() {
    if (this->project_setup_dialog_level() == nullptr) {
        throw std::runtime_error("dialog_level is null.");
    }

    this->_level->set_value(std::nullopt);
}

std::shared_ptr<project_setup_dialog_level> const &app_dialog_sub_level_router::project_setup_dialog_level() const {
    return get_level<ae::project_setup_dialog_level>(this->_level->value());
}

observing::syncable app_dialog_sub_level_router::observe(
    std::function<void(std::optional<app_dialog_sub_level> const &)> &&handler) {
    return this->_level->observe(
        [handler = std::move(handler)](std::optional<app_dialog_sub_level> const &level) { handler(level); });
}
