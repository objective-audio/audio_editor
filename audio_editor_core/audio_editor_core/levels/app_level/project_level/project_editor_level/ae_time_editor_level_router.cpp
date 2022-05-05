//
//  ae_time_editor_level_router.cpp
//

#include "ae_time_editor_level_router.h"

#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_editor_level_router> time_editor_level_router::make_shared(std::string const &identifier) {
    auto shared = std::shared_ptr<time_editor_level_router>(new time_editor_level_router{identifier});
    shared->_weak_pool = shared;
    return shared;
}

time_editor_level_router::time_editor_level_router(std::string const &identifier)
    : _identifier(identifier),
      _level(observing::value ::holder<std::shared_ptr<time_editor_level>>::make_shared(nullptr)) {
}

void time_editor_level_router::add_level(number_components const &components,
                                         std::optional<std::size_t> const unit_idx) {
    if (this->_level->value() != nullptr) {
        throw std::runtime_error("level is not null.");
    }

    this->_level->set_value(time_editor_level::make_shared(this->_identifier, components, unit_idx));
}

void time_editor_level_router::remove_level() {
    if (this->_level->value() == nullptr) {
        throw std::runtime_error("level is null.");
    }

    this->_level->set_value(nullptr);
}

std::shared_ptr<time_editor_level> const &time_editor_level_router::level() {
    return this->_level->value();
}

observing::syncable time_editor_level_router::observe_level(
    std::function<void(std::shared_ptr<time_editor_level> const &)> &&handler) {
    return this->_level->observe(std::move(handler));
}
