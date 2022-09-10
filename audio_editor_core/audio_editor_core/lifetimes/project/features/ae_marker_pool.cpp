//
//  ae_marker_pool.cpp
//

#include "ae_marker_pool.h"

#include <audio_editor_core/ae_database.h>
#include <cpp_utils/yas_assertion.h>
#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_pool> marker_pool::make_shared(database *database) {
    return std::make_shared<marker_pool>(database);
}

marker_pool::marker_pool(database *database)
    : _database(database), _markers(observing::map::holder<frame_index_t, marker>::make_shared()) {
}

void marker_pool::revert_markers(std::vector<marker> &&markers) {
    this->_markers->replace(to_map<frame_index_t>(std::move(markers), [](auto const &marker) { return marker.frame; }));
}

void marker_pool::insert_marker(marker const &marker) {
    if (!this->_markers->contains(marker.frame)) {
        this->_markers->insert_or_replace(marker.frame, marker);
    } else {
        assertion_failure_if_not_test();
    }
}

void marker_pool::replace_marker(marker const &marker) {
    if (this->_markers->contains(marker.frame)) {
        this->_markers->insert_or_replace(marker.frame, marker);
    } else {
        assertion_failure_if_not_test();
    }
}

void marker_pool::erase_at(frame_index_t const frame) {
    if (this->_markers->contains(frame)) {
        this->_markers->erase(frame);
    } else {
        assertion_failure_if_not_test();
    }
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
        this->insert_marker({{}, new_frame, ""});
    } else {
        assertion_failure_if_not_test();
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

std::optional<marker> marker_pool::marker_for_frame(frame_index_t const frame) const {
    if (this->_markers->elements().contains(frame)) {
        return this->_markers->elements().at(frame);
    }
    return std::nullopt;
}

std::optional<marker> marker_pool::marker_for_id(identifier const &identifier) const {
    for (auto const &pair : this->markers()) {
        if (pair.second.identifier == identifier) {
            return pair.second;
        }
    }
    return std::nullopt;
}

std::optional<frame_index_t> marker_pool::next_jumpable_frame(frame_index_t const frame) const {
    auto const &markers = this->markers();
    auto upper_it = markers.upper_bound(frame);
    if (upper_it != markers.end()) {
        return upper_it->first;
    }
    return std::nullopt;
}

std::optional<frame_index_t> marker_pool::previous_jumpable_frame(frame_index_t const frame) const {
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
            return marker_pool_event{.type = marker_pool_event_type::any,
                                     .inserted = std::nullopt,
                                     .erased = std::nullopt,
                                     .markers = this->_markers->elements()};
        });

        this->_markers
            ->observe([this](auto const &event) {
                switch (event.type) {
                    case observing::map::event_type::any:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::reverted,
                                                               .inserted = std::nullopt,
                                                               .erased = std::nullopt,
                                                               .markers = event.elements});
                        break;
                    case observing::map::event_type::inserted:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::inserted,
                                                               .inserted = *event.inserted,
                                                               .erased = std::nullopt,
                                                               .markers = event.elements});
                        break;
                    case observing::map::event_type::erased:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::erased,
                                                               .inserted = std::nullopt,
                                                               .erased = *event.erased,
                                                               .markers = event.elements});
                        break;
                    case observing::map::event_type::replaced:
                        this->_fetcher->push(marker_pool_event{.type = marker_pool_event_type::replaced,
                                                               .inserted = *event.inserted,
                                                               .erased = *event.erased,
                                                               .markers = event.elements});
                        break;
                }
            })
            .end()
            ->add_to(this->_pool);
    }

    return this->_fetcher->observe(std::move(handler));
}
