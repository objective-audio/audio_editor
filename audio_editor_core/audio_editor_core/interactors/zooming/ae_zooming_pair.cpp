//
//  ae_zooming_pair.cpp
//

#include "ae_zooming_pair.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<zooming_pair> zooming_pair::make_shared() {
    return std::shared_ptr<zooming_pair>(new zooming_pair{});
}

zooming_pair::zooming_pair() : horizontal(zooming::make_shared()), vertical(zooming::make_shared()) {
    this->_scale_fetcher = observing::fetcher<zooming_scale>::make_shared([this] { return this->scale(); });

    this->horizontal->observe_scale([this](double const &) { this->_scale_fetcher->push(); })
        .end()
        ->add_to(this->_pool);

    this->vertical->observe_scale([this](double const &) { this->_scale_fetcher->push(); }).end()->add_to(this->_pool);
}

zooming_scale zooming_pair::scale() const {
    return zooming_scale{.horizontal = this->horizontal->scale(), .vertical = this->vertical->scale()};
}

observing::syncable zooming_pair::observe_scale(std::function<void(zooming_scale const &)> &&handler) {
    return this->_scale_fetcher->observe(std::move(handler));
}
