//
//  ae_resource_pool_private.h
//

#pragma once

#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_stl_utils.h>

namespace yas::ae {
template <typename Resource>
std::shared_ptr<resource_pool<Resource>> resource_pool<Resource>::make_shared() {
    return std::shared_ptr<resource_pool<Resource>>(new resource_pool<Resource>{});
}

template <typename Resource>
resource_pool<Resource>::resource_pool() {
    this->_fetcher = observing::fetcher<resource_pool_event<Resource>>::make_shared([this] {
        return resource_pool_event<Resource>{.type = resource_pool_event_type::fetched, .locations = this->_locations};
    });
}

template <typename Resource>
void resource_pool<Resource>::replace_all(std::vector<Resource> const &locations) {
    this->_locations.clear();
    this->_locations = map<std::optional<Resource>>(locations, [](auto const &location) { return location; });

    this->_fetcher->push({.type = resource_pool_event_type::replaced, .locations = this->_locations});
}

template <typename Resource>
void resource_pool<Resource>::update_all(std::vector<Resource> const &locations) {
    std::vector<std::pair<std::size_t, Resource>> inserted;

    // 元のlocationsに無いものをinsertedに保持。indexはまだ
    for (auto const &location : locations) {
        auto const contains = contains_if(this->_locations, [&location](auto const &prev) {
            return prev.has_value() && prev.value().identifier == location.identifier;
        });

        if (!contains) {
            inserted.emplace_back(0, location);
        }
    }

    std::vector<std::pair<std::size_t, Resource>> erased;
    std::vector<std::size_t> null_indices;

    // 引数のlocationsに無いものを削除してerasedに保持。空のindexをnull_indicesに保持
    auto each = make_fast_each(this->_locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &prev = this->_locations.at(idx);

        if (prev.has_value()) {
            auto const contains = contains_if(locations, [&prev_id = prev.value().identifier](auto const &location) {
                return location.identifier == prev_id;
            });

            if (!contains) {
                erased.emplace_back(idx, prev.value());
                this->_locations.at(idx) = std::nullopt;
                null_indices.emplace_back(idx);
            }
        } else {
            null_indices.emplace_back(idx);
        }
    }

    // 空のindexから順にinsertedのものを挿入
    each = make_fast_each(inserted.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location_pair = inserted.at(idx);

        if (idx < null_indices.size()) {
            auto const inserted_idx = null_indices.at(idx);
            this->_locations.at(inserted_idx) = location_pair.second;
            inserted.at(idx) = std::make_pair(inserted_idx, location_pair.second);
        } else {
            this->_locations.emplace_back(location_pair.second);
            inserted.at(idx) = std::make_pair(this->_locations.size() - 1, location_pair.second);
        }
    }

    // 変更があれば通知
    if (!inserted.empty() || !erased.empty()) {
        this->_fetcher->push({.type = resource_pool_event_type::updated,
                              .locations = this->_locations,
                              .inserted = std::move(inserted),
                              .erased = std::move(erased)});
    }
}

template <typename Resource>
void resource_pool<Resource>::erase(identifier const &erase_id) {
    auto each = make_fast_each(this->_locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = this->_locations.at(idx);
        if (location.has_value() && location.value().identifier == erase_id) {
            std::vector<std::pair<std::size_t, Resource>> erased{{idx, location.value()}};

            this->_locations.at(idx) = std::nullopt;

            this->_fetcher->push({.type = resource_pool_event_type::updated,
                                  .locations = this->_locations,
                                  .erased = std::move(erased)});
            break;
        }
    }
}

template <typename Resource>
void resource_pool<Resource>::insert(Resource const &location) {
    std::vector<std::pair<std::size_t, Resource>> inserted;

    auto each = make_fast_each(this->_locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &inserting = this->_locations.at(idx);
        if (inserting.has_value()) {
            if (inserting.value().identifier == location.identifier) {
                return;
            }
        } else {
            inserted.emplace_back(idx, location);
            this->_locations.at(idx) = location;
            break;
        }
    }

    if (inserted.empty()) {
        this->_locations.emplace_back(location);
        inserted.emplace_back(this->_locations.size() - 1, location);
    }

    this->_fetcher->push(
        {.type = resource_pool_event_type::updated, .locations = this->_locations, .inserted = std::move(inserted)});
}

template <typename Resource>
std::vector<std::optional<Resource>> const &resource_pool<Resource>::locations() const {
    return this->_locations;
}

template <typename Resource>
observing::syncable resource_pool<Resource>::observe_event(
    std::function<void(resource_pool_event<Resource> const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}
}  // namespace yas::ae
