//
//  ae_marker_pool.cpp
//

#include "ae_marker_pool.h"

#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

marker_pool::marker_pool() : _markers(observing::map::holder<frame_index_t, marker>::make_shared()) {
}

void marker_pool::revert_markers(std::vector<marker> &&markers) {
    this->_markers->replace(to_map<frame_index_t>(std::move(markers), [](auto const &marker) { return marker.frame; }));
}

void marker_pool::insert_marker(marker const &marker) {
    this->_markers->insert_or_replace(marker.frame, marker);
}

void marker_pool::erase_at(frame_index_t const frame) {
    this->_markers->erase(frame);
}

void marker_pool::erase_marker(marker const &marker) {
    this->erase_at(marker.frame);
}

void marker_pool::erase_range(time::range const range) {
    auto const filtered = filter(this->_markers->elements(), [&range](auto const &pair) {
        return range.frame <= pair.first && pair.first < range.next_frame();
    });

    for (auto const &pair : filtered) {
        this->erase_at(pair.first);
    }
}

void marker_pool::move_at(frame_index_t const frame, frame_index_t const new_frame) {
    if (this->_markers->contains(frame)) {
        this->erase_at(frame);
        this->insert_marker({.frame = new_frame});
    }
}

void marker_pool::move_offset_from(frame_index_t const from, frame_index_t const offset) {
    auto const filtered = filter(this->_markers->elements(), [&from](auto const &pair) { return from <= pair.first; });

    for (auto const &pair : filtered) {
        this->move_at(pair.first, pair.first + offset);
    }
}

marker_map_t const &marker_pool::markers() const {
    return this->_markers->elements();
}

std::optional<marker> marker_pool::marker_at(std::size_t const idx) const {
    auto const &markers = this->markers();
    if (markers.size() <= idx) {
        return std::nullopt;
    }

    auto iterator = markers.begin();
    auto each = make_fast_each(idx);
    while (yas_each_next(each)) {
        ++iterator;
    }

    return iterator->second;
}

std::optional<frame_index_t> marker_pool::next_edge(frame_index_t const frame) const {
    auto const &markers = this->markers();
    auto upper_it = markers.upper_bound(frame);
    if (upper_it != markers.end()) {
        return upper_it->first;
    }
    return std::nullopt;
}

std::optional<frame_index_t> marker_pool::previous_edge(frame_index_t const frame) const {
    auto const &markers = this->markers();
    for (auto it = markers.rbegin(); it != markers.rend(); ++it) {
        if (it->first < frame) {
            return it->first;
        }
    }
    return std::nullopt;
}

observing::syncable marker_pool::observe_event(std::function<void(marker_pool_event const &)> &&handler) {
    if (!this->_fetcher) {
        this->_fetcher = observing::fetcher<marker_pool_event>::make_shared([this] {
            return marker_pool_event{.type = marker_pool_event_type::any, .markers = this->_markers->elements()};
        });

        this->_markers
            ->observe([this](auto const &event) {
                switch (event.type) {
                    case observing::map::event_type::any:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::reverted,
                                                               .marker = std::nullopt,
                                                               .markers = event.elements});
                        break;
                    case observing::map::event_type::inserted:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::inserted,
                                                               .marker = *event.inserted,
                                                               .markers = event.elements});
                        break;
                    case observing::map::event_type::erased:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::erased,
                                                               .marker = *event.erased,
                                                               .markers = event.elements});
                        break;
                    case observing::map::event_type::replaced:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::erased,
                                                               .marker = *event.erased,
                                                               .markers = event.elements});
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::inserted,
                                                               .marker = *event.inserted,
                                                               .markers = event.elements});
                        break;
                }
            })
            .end()
            ->add_to(this->_pool);
    }

    return this->_fetcher->observe(std::move(handler));
}

std::shared_ptr<marker_pool> marker_pool::make_shared() {
    return std::shared_ptr<marker_pool>(new marker_pool{});
}
