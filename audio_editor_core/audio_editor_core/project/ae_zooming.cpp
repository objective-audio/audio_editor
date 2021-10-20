//
//  ae_zooming.cpp
//

#include "ae_zooming.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<zooming> zooming::make_shared() {
    return std::shared_ptr<zooming>(new zooming{});
}

zooming::zooming() : _scale(observing::value::holder<double>::make_shared(1.0)) {
}

void zooming::begin() {
    this->_began_scale = this->_scale->value();
}

void zooming::set_magnification(double const magnification) {
    if (auto const began_scale = this->_began_scale) {
        double const rate = std::pow(10.0, magnification / 2.0);
        double const value = std::min(std::max(began_scale.value() * rate, 0.1), 10.0);
        this->_scale->set_value(value);
    }
}

void zooming::end() {
    this->_began_scale = std::nullopt;
}

double zooming::scale() const {
    return this->_scale->value();
}

observing::syncable zooming::observe_scale(std::function<void(double const &)> &&handler) {
    return this->_scale->observe(std::move(handler));
}
