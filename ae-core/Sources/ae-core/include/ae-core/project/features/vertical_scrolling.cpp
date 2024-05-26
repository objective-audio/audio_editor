//
//  vertical_scrolling.cpp
//

#include "vertical_scrolling.hpp"

using namespace yas;
using namespace yas::ae;

vertical_scrolling::vertical_scrolling() : _position(observing::value::holder<double>::make_shared(0.0)) {
}

double vertical_scrolling::track_position() const {
    return this->_position->value();
}

track_index_t vertical_scrolling::track() const {
    return std::round(this->track_position());
}

double vertical_scrolling::position_offset() const {
    return this->track_position() - this->track();
}

observing::syncable vertical_scrolling::observe_track_position(std::function<void(double const &)> &&handler) {
    return this->_position->observe(std::move(handler));
}

observing::syncable vertical_scrolling::observe_track(std::function<void(track_index_t const &)> &&handler) {
    return this->_position->observe(
        [handler = std::move(handler)](double const &position) { handler(std::round(position)); });
}

void vertical_scrolling::begin() {
    this->end();

    this->_is_began = true;
}

void vertical_scrolling::set_delta_position(double const delta_track) {
    if (this->_is_began) {
        this->_position->set_value(this->_position->value() + delta_track);
    }
}

void vertical_scrolling::end() {
    if (this->_is_began) {
        this->_is_began = false;
    }
}

bool vertical_scrolling::is_began() const {
    return this->_is_began;
}

void vertical_scrolling::move_to_track(track_index_t const track) {
    this->end();

    this->_position->set_value(track);
}

void vertical_scrolling::move_to_previous_track(track_index_t const offset) {
    if (offset <= 0) {
        throw std::underflow_error("offset is less than 1.");
    }

    this->move_to_track(this->track() - offset);
}

void vertical_scrolling::move_to_next_track(track_index_t const offset) {
    if (offset <= 0) {
        throw std::underflow_error("offset is less than 1.");
    }

    this->move_to_track(this->track() + offset);
}
