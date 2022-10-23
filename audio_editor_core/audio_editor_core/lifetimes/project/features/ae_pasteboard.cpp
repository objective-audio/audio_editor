//
//  ae_pasteboard.cpp
//

#include "ae_pasteboard.h"

using namespace yas;
using namespace yas::ae;

static std::vector<pasting_file_module_object> const _empty_modules;

std::shared_ptr<pasteboard> pasteboard::make_shared() {
    return std::make_shared<pasteboard>();
}

pasteboard::pasteboard()
    : _event_fetcher(observing::fetcher<pasteboard_event>::make_shared([] { return pasteboard_event::fetched; })) {
}

std::vector<pasting_file_module_object> const &pasteboard::file_modules() const {
    if (this->_value.has_value()) {
        auto const &pasting_value = this->_value.value();
        if (std::holds_alternative<std::vector<pasting_file_module_object>>(pasting_value)) {
            return std::get<std::vector<pasting_file_module_object>>(pasting_value);
        }
    }

    return _empty_modules;
}

void pasteboard::set_file_modules(std::vector<pasting_file_module_object> const &modules) {
    this->_value = modules;
    this->_event_fetcher->push(pasteboard_event::file_modules);
}

std::optional<pasting_value> const &pasteboard::value() const {
    return this->_value;
}

bool pasteboard::can_clear() const {
    return this->_value.has_value();
}

void pasteboard::clear() {
    if (!this->can_clear()) {
        return;
    }

    this->_value = std::nullopt;
    this->_event_fetcher->push(pasteboard_event::cleared);
}

observing::syncable pasteboard::observe_event(std::function<void(pasteboard_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
