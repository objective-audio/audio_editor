//
//  ae_pasteboard.cpp
//

#include "ae_pasteboard.h"

using namespace yas;
using namespace yas::ae;

static std::vector<pasting_file_module_object> const _empty_modules;
static std::vector<pasting_marker_object> const _empty_markers;

pasteboard::pasteboard()
    : _event_fetcher(observing::fetcher<pasteboard_event>::make_shared([] { return pasteboard_event::fetched; })) {
}

bool pasteboard::has_file_modules() const {
    if (this->_value.has_value()) {
        return std::holds_alternative<std::vector<pasting_file_module_object>>(this->_value.value());
    } else {
        return false;
    }
}

std::vector<pasting_file_module_object> const &pasteboard::file_modules() const {
    if (this->has_file_modules()) {
        return std::get<std::vector<pasting_file_module_object>>(this->_value.value());
    } else {
        return _empty_modules;
    }
}

void pasteboard::set_file_modules(std::vector<pasting_file_module_object> const &modules) {
    if (!this->has_file_modules()) {
        this->clear();
    }

    this->_value = modules;
    this->_event_fetcher->push(pasteboard_event::file_modules_replaced);
}

bool pasteboard::has_markers() const {
    if (this->_value.has_value()) {
        return std::holds_alternative<std::vector<pasting_marker_object>>(this->_value.value());
    } else {
        return false;
    }
}

std::vector<pasting_marker_object> const &pasteboard::markers() const {
    if (this->has_markers()) {
        return std::get<std::vector<pasting_marker_object>>(this->_value.value());
    } else {
        return _empty_markers;
    }
}

void pasteboard::set_markers(std::vector<pasting_marker_object> const &markers) {
    if (!this->has_markers()) {
        this->clear();
    }

    this->_value = markers;
    this->_event_fetcher->push(pasteboard_event::markers_replaced);
}

std::optional<pasting_value> const &pasteboard::value() const {
    return this->_value;
}

bool pasteboard::can_clear() const {
    return this->_value.has_value();
}

void pasteboard::clear() {
    if (this->_value.has_value()) {
        auto const &pasting_value = this->_value.value();
        if (std::holds_alternative<std::vector<pasting_file_module_object>>(pasting_value)) {
            this->_value = std::nullopt;
            this->_event_fetcher->push(pasteboard_event::file_modules_cleared);
        } else if (std::holds_alternative<std::vector<pasting_marker_object>>(pasting_value)) {
            this->_value = std::nullopt;
            this->_event_fetcher->push(pasteboard_event::markers_cleared);
        }
    }
}

observing::syncable pasteboard::observe_event(std::function<void(pasteboard_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
