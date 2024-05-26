//
//  pasteboard.cpp
//

#include "pasteboard.h"

using namespace yas;
using namespace yas::ae;

pasteboard::pasteboard()
    : _event_fetcher(observing::fetcher<pasteboard_event>::make_shared([] { return pasteboard_event::fetched; })) {
}

bool pasteboard::has_modules() const {
    return !this->_modules.empty();
}

std::vector<pasting_module_object> const &pasteboard::modules() const {
    return this->_modules;
}

void pasteboard::set_modules(std::vector<pasting_module_object> const &modules) {
    this->_modules = modules;
    this->_event_fetcher->push(pasteboard_event::modules_replaced);
}

bool pasteboard::has_markers() const {
    return !this->_markers.empty();
}

std::vector<pasting_marker_object> const &pasteboard::markers() const {
    return this->_markers;
}

void pasteboard::set_markers(std::vector<pasting_marker_object> const &markers) {
    this->_markers = markers;
    this->_event_fetcher->push(pasteboard_event::markers_replaced);
}

bool pasteboard::can_clear() const {
    return !this->_modules.empty() || !this->_markers.empty();
}

void pasteboard::clear() {
    if (!this->_modules.empty()) {
        this->_modules.clear();
        this->_event_fetcher->push(pasteboard_event::modules_cleared);
    }

    if (!this->_markers.empty()) {
        this->_markers.clear();
        this->_event_fetcher->push(pasteboard_event::markers_cleared);
    }
}

observing::syncable pasteboard::observe_event(std::function<void(pasteboard_event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
