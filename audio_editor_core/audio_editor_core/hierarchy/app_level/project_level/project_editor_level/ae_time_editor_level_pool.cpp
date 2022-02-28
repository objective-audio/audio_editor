//
//  ae_time_editor_level_pool.cpp
//

#include "ae_time_editor_level_pool.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_level_pool> time_editor_level_pool::make_shared(std::string const &identifier) {
    return std::shared_ptr<time_editor_level_pool>(new time_editor_level_pool{identifier});
}

time_editor_level_pool::time_editor_level_pool(std::string const &identifier)
    : _identifier(identifier),
      _level(observing::value ::holder<std::shared_ptr<time_editor_level>>::make_shared(nullptr)) {
}

void time_editor_level_pool::add_level(number_components const &components) {
    if (this->_level->value() != nullptr) {
        throw std::runtime_error("level is not null.");
    }

    this->_level->set_value(time_editor_level::make_shared(components));
}

void time_editor_level_pool::remove_level() {
    if (this->_level->value() == nullptr) {
        throw std::runtime_error("level is null.");
    }

    this->_level->set_value(nullptr);
}

std::shared_ptr<time_editor_level> const &time_editor_level_pool::level() {
    return this->_level->value();
}

observing::syncable time_editor_level_pool::observe_level(
    std::function<void(std::shared_ptr<time_editor_level> const &)> &&handler) {
    return this->_level->observe(std::move(handler));
}
