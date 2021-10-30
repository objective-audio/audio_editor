//
//  ae_scrolling.cpp
//

#include "ae_scrolling.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<scrolling> scrolling::make_shared() {
    return std::shared_ptr<scrolling>(new scrolling{});
}

scrolling::scrolling() : _notifier(observing::notifier<scrolling_event>::make_shared()) {
}

bool scrolling::is_enabled() const {
    return this->_is_enabled;
}

void scrolling::set_enabled(bool const is_enabled) {
    if (this->_is_enabled != is_enabled) {
        this->_is_enabled = is_enabled;
        this->end();
    }
}

void scrolling::begin() {
    this->end();

    if (this->_is_enabled) {
        this->_is_began = true;
        this->_notifier->notify({.state = scrolling_state::began, .delta_time = 0.0});
    }
}

void scrolling::set_delta_time(double const value) {
    if (this->_is_began) {
        this->_notifier->notify({.state = scrolling_state::changed, .delta_time = value});
    }
}

void scrolling::end() {
    if (this->_is_began) {
        this->_is_began = false;
        this->_notifier->notify({.state = scrolling_state::ended, .delta_time = 0.0});
    }
}

bool scrolling::is_began() const {
    return this->_is_began;
}

observing::endable scrolling::observe(std::function<void(scrolling_event const &)> &&handler) {
    return this->_notifier->observe(std::move(handler));
}
