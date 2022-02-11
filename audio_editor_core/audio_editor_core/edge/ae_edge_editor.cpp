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
    if (this->_edge != edge) {
        this->_edge = edge;

        this->_fetcher->push({.type = edge_editor_event_type::updated, .edge = this->_edge});
    }
}

void edge_editor::revert_edge(ae::edge const &edge) {
    if (this->_edge != edge) {
        this->_edge = edge;

        this->_fetcher->push({.type = edge_editor_event_type::reverted, .edge = this->_edge});
    }
}

observing::syncable edge_editor::observe_event(std::function<void(edge_editor_event const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}
