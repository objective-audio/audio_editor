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

marker_pool::marker_pool(database_for_marker_pool *database)
    : _database(database), _markers(observing::map::holder<marker_index, marker_object>::make_shared()) {
}

void marker_pool::revert_markers(std::vector<marker_object> &&markers) {
    this->_markers->replace(
        to_map<marker_index>(std::move(markers), [](auto const &marker) { return marker.index(); }));
}

std::optional<marker_index> marker_pool::insert_marker(frame_index_t const frame, std::string const &name) {
    if (auto const marker = this->_database->add_marker(frame, name).object(); marker.has_value()) {
        auto index = marker.value().index();
        this->_markers->insert_or_replace(index, marker.value());
        return index;
    } else {
        assertion_failure_if_not_test();
    }

    return std::nullopt;
}

void marker_pool::update_marker(marker_index const index, marker_object const &new_marker) {
    if (this->_markers->contains(index)) {
        this->_database->update_marker(index.object_id, new_marker);
        if (index.frame != new_marker.value.frame) {
            this->_markers->erase(index);
        }
        this->_markers->insert_or_replace(new_marker.index(), new_marker);
    } else {
        assertion_failure_if_not_test();
    }
}

void marker_pool::erase(marker_index const &index) {
    if (this->_markers->contains(index)) {
        this->_markers->erase(index);
        this->_database->remove_marker(index.object_id);
    } else {
        assertion_failure_if_not_test();
    }
}

void marker_pool::erase_at(frame_index_t const frame) {
    auto const markers = this->markers_for_frame(frame);

    if (!markers.empty()) {
        for (auto const &marker : markers) {
            this->erase(marker.index());
        }
    } else {
        assertion_failure_if_not_test();
    }
}

void marker_pool::erase_range(time::range const range) {
    auto const filtered = filter(this->_markers->elements(), [&range](auto const &pair) {
        return range.frame <= pair.second.value.frame && pair.second.value.frame < range.next_frame();
    });

    for (auto const &pair : filtered) {
        this->erase(pair.first);
    }
}

void marker_pool::move_at(marker_index const &index, frame_index_t const new_frame) {
    if (auto const marker = this->marker_for_index(index); marker.has_value()) {
        auto marker_value = marker.value();
        marker_value.value.frame = new_frame;
        this->update_marker(index, marker_value);
    } else {
        assertion_failure_if_not_test();
    }
}

void marker_pool::move_offset_from(frame_index_t const from, frame_index_t const offset) {
    auto const filtered =
        filter(this->_markers->elements(), [&from](auto const &pair) { return from <= pair.first.frame; });

    for (auto const &pair : filtered) {
        this->move_at(pair.first, pair.first.frame + offset);
    }
}

marker_map_t const &marker_pool::markers() const {
    return this->_markers->elements();
}

std::optional<marker_object> marker_pool::marker_at(std::size_t const idx) const {
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

std::optional<marker_object> marker_pool::marker_for_index(marker_index const &index) const {
    if (this->_markers->elements().contains(index)) {
        return this->_markers->elements().at(index);
    }
    return std::nullopt;
}

std::vector<marker_object> marker_pool::markers_for_frame(frame_index_t const &frame) const {
    std::vector<marker_object> markers;

    for (auto const &pair : this->markers()) {
        if (pair.first.frame == frame) {
            markers.emplace_back(pair.second);
        }
    }

    return markers;
}

std::optional<marker_object> marker_pool::marker_for_id(object_id const &identifier) const {
    for (auto const &pair : this->markers()) {
        if (pair.second.identifier == identifier) {
            return pair.second;
        }
    }
    return std::nullopt;
}

bool marker_pool::marker_exists_for_frame(frame_index_t const frame) const {
    for (auto const &pair : this->markers()) {
        if (pair.first.frame == frame) {
            return true;
        }
    }
    return false;
}

std::optional<frame_index_t> marker_pool::next_jumpable_frame(frame_index_t const frame) const {
    for (auto const &pair : this->markers()) {
        if (frame < pair.first.frame) {
            return pair.first.frame;
        }
    }
    return std::nullopt;
}

std::optional<frame_index_t> marker_pool::previous_jumpable_frame(frame_index_t const frame) const {
    auto const &markers = this->markers();
    for (auto it = markers.rbegin(); it != markers.rend(); ++it) {
        if (it->first.frame < frame) {
            return it->first.frame;
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
