//
//  ae_marker_pool.cpp
//

#include "ae_marker_pool.h"

#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

marker_pool::marker_pool() : _markers(observing::map::holder<proc::frame_index_t, marker>::make_shared()) {
}

void marker_pool::replace_markers(std::vector<marker> &&markers) {
    this->_markers->replace(
        to_map<proc::frame_index_t>(std::move(markers), [](auto const &marker) { return marker.frame; }));
}

void marker_pool::insert_marker(marker const &marker) {
    this->_markers->insert_or_replace(marker.frame, marker);
}

void marker_pool::erase_at(proc::frame_index_t const frame) {
    this->_markers->erase(frame);
}

void marker_pool::erase_marker(marker const &marker) {
    this->erase_at(marker.frame);
}

std::map<proc::frame_index_t, marker> const &marker_pool::markers() const {
    return this->_markers->elements();
}

observing::syncable marker_pool::observe_event(std::function<void(marker_pool_event const &)> &&handler) {
    return this->_markers->observe([handler = std::move(handler)](auto const &event) {
        switch (event.type) {
            case observing::map::event_type::any:
                handler(marker_pool_event{.type = marker_pool_event_type::any, .markers = event.elements});
                break;
            case observing::map::event_type::inserted:
                handler(marker_pool_event{
                    .type = marker_pool_event_type::inserted, .marker = *event.inserted, .markers = event.elements});
                break;
            case observing::map::event_type::erased:
                handler(marker_pool_event{
                    .type = marker_pool_event_type::erased, .marker = *event.erased, .markers = event.elements});
                break;
            case observing::map::event_type::replaced:
                handler(marker_pool_event{
                    .type = marker_pool_event_type::erased, .marker = *event.erased, .markers = event.elements});
                handler(marker_pool_event{
                    .type = marker_pool_event_type::inserted, .marker = *event.inserted, .markers = event.elements});
                break;
        }
    });
}

std::shared_ptr<marker_pool> marker_pool::make_shared() {
    return std::shared_ptr<marker_pool>(new marker_pool{});
}
