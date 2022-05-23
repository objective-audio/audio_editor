//
//  ae_app_dialog_level_router.cpp
//

#include "ae_app_dialog_level_router.h"

#include <audio_editor_core/ae_app_dialog_level.h>

using namespace yas;
using namespace yas::ae;

static std::shared_ptr<app_dialog_level> const _null_dialog_level;

std::shared_ptr<app_dialog_level_router> app_dialog_level_router::make_shared() {
    return std::make_shared<app_dialog_level_router>();
}

app_dialog_level_router::app_dialog_level_router()
    : _level(observing::value::holder<std::optional<std::shared_ptr<app_dialog_level>>>::make_shared(std::nullopt)) {
}

void app_dialog_level_router::add_dialog(app_dialog_content const content) {
    if (this->_level->value().has_value()) {
        throw std::runtime_error("level is not null.");
    }

    this->_level->set_value(app_dialog_level::make_shared(content));
}

void app_dialog_level_router::remove_dialog(app_dialog_content const content) {
    if (this->dialog_level() == nullptr) {
        throw std::runtime_error("dialog_level is null.");
    }

    this->_level->set_value(std::nullopt);
}

std::shared_ptr<app_dialog_level> const &app_dialog_level_router::dialog_level() const {
    if (this->_level->value().has_value()) {
        return this->_level->value().value();
    } else {
        return _null_dialog_level;
    }
}

observing::syncable app_dialog_level_router::observe(
    std::function<void(std::optional<std::shared_ptr<app_dialog_level>> const &)> &&handler) {
    return this->_level->observe(std::move(handler));
}
