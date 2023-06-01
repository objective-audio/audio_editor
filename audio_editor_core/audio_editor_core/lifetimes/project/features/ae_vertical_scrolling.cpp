//
//  ae_vertical_scrolling.cpp
//

#include "ae_vertical_scrolling.hpp"

using namespace yas;
using namespace yas::ae;

vertical_scrolling::vertical_scrolling() : _track(0.0) {
}

double vertical_scrolling::track() const {
    return this->_track;
}

void vertical_scrolling::begin() {
    this->end();

    this->_is_began = true;
}

void vertical_scrolling::set_delta_track(double const delta_track) {
    if (this->_is_began) {
        this->_track += delta_track;
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
