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
    : _level(observing::value::holder<std::shared_ptr<app_dialog_level>>::make_shared(nullptr)) {
}

void app_dialog_level_router::add_dialog(app_dialog_content const content) {
    if (this->_level->value()) {
        throw std::runtime_error("level is not null.");
    }

    this->_level->set_value(app_dialog_level::make_shared(content));
}

void app_dialog_level_router::remove_dialog(app_dialog_content const content) {
    auto const &level = this->dialog_level();

    if (level == nullptr || level->content != content) {
        throw std::runtime_error("dialog_level is null.");
    }

    this->_level->set_value(nullptr);
}

std::shared_ptr<app_dialog_level> const &app_dialog_level_router::dialog_level() const {
    if (this->_level->value()) {
        return this->_level->value();
    } else {
        return _null_dialog_level;
    }
}

observing::syncable app_dialog_level_router::observe(
    std::function<void(std::optional<app_dialog_content> const &)> &&handler) {
    return this->_level->observe([handler = std::move(handler)](std::shared_ptr<app_dialog_level> const &level) {
        if (level) {
            handler(level->content);
        } else {
            handler(std::nullopt);
        }
    });
}
