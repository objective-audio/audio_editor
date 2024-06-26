//
//  horizontal_scrolling.cpp
//

#include "horizontal_scrolling.h"

using namespace yas;
using namespace yas::ae;

horizontal_scrolling::horizontal_scrolling() : _notifier(observing::notifier<scrolling_event>::make_shared()) {
}

void horizontal_scrolling::begin() {
    if (this->_is_enabled) {
        this->end();

        this->_is_began = true;
        this->_notifier->notify({.state = scrolling_state::began, .delta_time = 0.0});
    }
}

void horizontal_scrolling::set_delta_time(double const value) {
    if (this->_is_enabled && this->_is_began) {
        this->_notifier->notify({.state = scrolling_state::changed, .delta_time = value});
    }
}

void horizontal_scrolling::end() {
    if (this->_is_began) {
        this->_is_began = false;
        this->_notifier->notify({.state = scrolling_state::ended, .delta_time = 0.0});
    }
}

bool horizontal_scrolling::is_began() const {
    return this->_is_began;
}

void horizontal_scrolling::set_is_enabled(bool const is_enabled) {
    if (this->_is_enabled != is_enabled) {
        this->_is_enabled = is_enabled;

        if (!is_enabled) {
            this->end();
        }
    }
}

bool horizontal_scrolling::is_enabled() const {
    return this->_is_enabled;
}

observing::endable horizontal_scrolling::observe(std::function<void(scrolling_event const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}
