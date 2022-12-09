//
//  ae_selected_marker_pool.cpp
//

#include "ae_selected_marker_pool.hpp"

#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

selected_marker_pool::selected_marker_pool() {
    this->_event_fetcher =
        observing::fetcher<event>::make_shared([this] { return event{.type = event_type::fetched}; });
}

selected_marker_map const &selected_marker_pool::markers() const {
    return this->_markers;
}

bool selected_marker_pool::contains(marker_index const &index) const {
    return this->_markers.contains(index);
}

void selected_marker_pool::toggle_marker(selected_marker_object const &marker) {
    auto const index = marker.index();

    if (this->_markers.contains(index)) {
        this->erase_marker(index);
    } else {
        this->insert_marker(marker);
    }
}

void selected_marker_pool::insert_marker(selected_marker_object const &marker) {
    auto index = marker.index();

    if (this->_markers.contains(index)) {
        assertion_failure_if_not_test();
        return;
    }

    this->_markers.emplace(index, marker);

    this->_event_fetcher->push({.type = event_type::inserted, .markers = {{index, marker}}});
}

void selected_marker_pool::erase_marker(marker_index const &index) {
    if (!this->_markers.contains(index)) {
        assertion_failure_if_not_test();
        return;
    }

    auto const erasing = this->_markers.at(index);

    this->_markers.erase(index);

    this->_event_fetcher->push({.type = event_type::erased, .markers = {{index, erasing}}});
}

bool selected_marker_pool::can_clear() const {
    return !this->_markers.empty();
}

void selected_marker_pool::clear() {
    if (!this->can_clear()) {
        return;
    }

    auto erased = std::move(this->_markers);

    this->_markers.clear();

    this->_event_fetcher->push({.type = event_type::erased, .markers = std::move(erased)});
}

observing::syncable selected_marker_pool::observe_event(std::function<void(event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
