//
//  ae_edge_editor.cpp
//

#include "ae_edge_editor.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_editor> edge_editor::make_shared() {
    return std::shared_ptr<edge_editor>(new edge_editor{});
}

edge_editor::edge_editor() : _edge(ae::edge::zero()) {
    this->_fetcher = observing::fetcher<edge_editor_event>::make_shared(
        [this] { return edge_editor_event{.type = edge_editor_event_type::fetched, .edge = this->_edge}; });
}

ae::edge const &edge_editor::edge() const {
    return this->_edge;
}

void edge_editor::set_edge(ae::edge const &edge) {
    this->_set_edge_and_notify(edge, edge_editor_event_type::updated);
}

void edge_editor::set_begin_frame(frame_index_t const begin_frame) {
    this->_set_edge_and_notify({.begin_frame = begin_frame, .end_frame = std::max(begin_frame, this->_edge.end_frame)},
                               edge_editor_event_type::updated);
}

void edge_editor::set_end_frame(frame_index_t const end_frame) {
    this->_set_edge_and_notify({.begin_frame = std::min(end_frame, this->_edge.begin_frame), .end_frame = end_frame},
                               edge_editor_event_type::updated);
}

void edge_editor::revert_edge(ae::edge const &edge) {
    this->_set_edge_and_notify(edge, edge_editor_event_type::reverted);
}

std::optional<frame_index_t> edge_editor::next_jumpable_frame(frame_index_t const frame) const {
    if (frame < this->_edge.begin_frame) {
        return this->_edge.begin_frame;
    } else if (frame < this->_edge.end_frame) {
        return this->_edge.end_frame;
    } else {
        return std::nullopt;
    }
}

std::optional<frame_index_t> edge_editor::previous_jumpable_frame(frame_index_t const frame) const {
    if (this->_edge.end_frame < frame) {
        return this->_edge.end_frame;
    } else if (this->_edge.begin_frame < frame) {
        return this->_edge.begin_frame;
    } else {
        return std::nullopt;
    }
}

observing::syncable edge_editor::observe_event(std::function<void(edge_editor_event const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}

void edge_editor::_set_edge_and_notify(ae::edge const &edge, edge_editor_event_type const event_type) {
    if (this->_edge != edge) {
        if (edge.end_frame < edge.begin_frame) {
            throw std::invalid_argument("invalid edge order.");
        }

        this->_edge = edge;

        this->_fetcher->push({.type = event_type, .edge = this->_edge});
    }
}
