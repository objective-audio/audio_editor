//
//  ae_edge_holder.cpp
//

#include "ae_edge_holder.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_holder> edge_holder::make_shared() {
    return std::make_shared<edge_holder>();
}

edge_holder::edge_holder() : _edge(ae::edge::zero()) {
    this->_fetcher = observing::fetcher<edge_holder_event>::make_shared(
        [this] { return edge_holder_event{.type = edge_holder_event_type::fetched, .edge = this->_edge}; });
}

ae::edge const &edge_holder::edge() const {
    return this->_edge;
}

void edge_holder::set_edge(ae::edge const &edge) {
    this->_set_edge_and_notify(edge, edge_holder_event_type::updated);
}

void edge_holder::set_begin_frame(frame_index_t const begin_frame) {
    this->_set_edge_and_notify({.begin_frame = begin_frame, .end_frame = std::max(begin_frame, this->_edge.end_frame)},
                               edge_holder_event_type::updated);
}

void edge_holder::set_end_frame(frame_index_t const end_frame) {
    this->_set_edge_and_notify({.begin_frame = std::min(end_frame, this->_edge.begin_frame), .end_frame = end_frame},
                               edge_holder_event_type::updated);
}

void edge_holder::revert_edge(ae::edge const &edge) {
    this->_set_edge_and_notify(edge, edge_holder_event_type::reverted);
}

std::optional<frame_index_t> edge_holder::next_jumpable_frame(frame_index_t const frame) const {
    if (frame < this->_edge.begin_frame) {
        return this->_edge.begin_frame;
    } else if (frame < this->_edge.end_frame) {
        return this->_edge.end_frame;
    } else {
        return std::nullopt;
    }
}

std::optional<frame_index_t> edge_holder::previous_jumpable_frame(frame_index_t const frame) const {
    if (this->_edge.end_frame < frame) {
        return this->_edge.end_frame;
    } else if (this->_edge.begin_frame < frame) {
        return this->_edge.begin_frame;
    } else {
        return std::nullopt;
    }
}

observing::syncable edge_holder::observe_event(std::function<void(edge_holder_event const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}

void edge_holder::_set_edge_and_notify(ae::edge const &edge, edge_holder_event_type const event_type) {
    if (this->_edge != edge) {
        if (edge.end_frame < edge.begin_frame) {
            throw std::invalid_argument("invalid edge order.");
        }

        this->_edge = edge;

        this->_fetcher->push({.type = event_type, .edge = this->_edge});
    }
}
