//
//  ae_zooming.cpp
//

#include "ae_zooming.h"

using namespace yas;
using namespace yas::ae;

zooming::zooming() : _scale(observing::value::holder<double>::make_shared(1.0)) {
}

void zooming::begin() {
    this->_began_scale = this->_scale->value();
    this->_magnification = 0.0;
}

void zooming::set_magnification(double const magnification) {
    auto const began_scale = this->_began_scale;
    auto const prev_magnification = this->_magnification;
    if (began_scale && prev_magnification) {
        auto const current_magnification = prev_magnification.value() + magnification;
        double const rate = std::pow(10.0, current_magnification / 2.0);
        double const value = std::min(std::max(began_scale.value() * rate, 0.1), 10.0);
        this->_magnification = current_magnification;
        this->_scale->set_value(value);
    }
}

void zooming::end() {
    this->_began_scale = std::nullopt;
    this->_magnification = std::nullopt;
}

double zooming::scale() const {
    return this->_scale->value();
}

observing::syncable zooming::observe_scale(std::function<void(double const &)> &&handler) {
    return this->_scale->observe(std::move(handler));
}
