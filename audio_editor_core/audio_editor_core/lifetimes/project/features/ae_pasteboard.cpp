//
//  ae_pasteboard.cpp
//

#include "ae_pasteboard.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<pasteboard> pasteboard::make_shared() {
    return std::make_shared<pasteboard>();
}

pasteboard::pasteboard()
    : _event_fetcher(observing::fetcher<pasteboard_event>::make_shared([] { return pasteboard_event::fetched; })) {
}

std::optional<pasting_file_module> pasteboard::file_module() const {
    if (this->_value.has_value()) {
        auto const &pasting_value = this->_value.value();

        if (std::holds_alternative<pasting_file_module>(pasting_value)) {
            return std::get<pasting_file_module>(pasting_value);
        }
    }

    return std::nullopt;
}

void pasteboard::set_file_module(pasting_file_module const &module) {
    this->_value = module;
    this->_event_fetcher->push(pasteboard_event::file_module);
}

std::optional<pasting_value> const &pasteboard::value() const {
    return this->_value;
}

void pasteboard::revert_value(std::optional<pasting_value> const &value) {
    this->_value = value;
    this->_event_fetcher->push(pasteboard_event::reverted);
}

void pasteboard::revert_data(std::string const &data) {
    if (auto file_module = pasting_file_module::make_value(data)) {
        this->_value = file_module;
    } else {
        this->_value = std::nullopt;
    }

    this->_event_fetcher->push(pasteboard_event::reverted);
}

observing::syncable pasteboard::observe_event(std::function<void(pasteboard_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
