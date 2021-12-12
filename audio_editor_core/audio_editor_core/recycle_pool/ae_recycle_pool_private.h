//
//  ae_recycle_pool_private.h
//

#pragma once

#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_stl_utils.h>

namespace yas::ae {
template <typename Element>
std::shared_ptr<recycle_pool<Element>> recycle_pool<Element>::make_shared() {
    return std::shared_ptr<recycle_pool<Element>>(new recycle_pool<Element>{});
}

template <typename Element>
recycle_pool<Element>::recycle_pool() {
    this->_fetcher = observing::fetcher<recycle_pool_event<Element>>::make_shared([this] {
        return recycle_pool_event<Element>{.type = recycle_pool_event_type::fetched, .elements = this->_elements};
    });
}

template <typename Element>
void recycle_pool<Element>::replace_all(std::vector<Element> const &locations) {
    this->_elements.clear();
    this->_elements = map<std::optional<Element>>(locations, [](auto const &location) { return location; });

    this->_fetcher->push({.type = recycle_pool_event_type::replaced, .elements = this->_elements});
}

template <typename Element>
void recycle_pool<Element>::update_all(std::vector<Element> const &locations) {
    std::vector<std::pair<std::size_t, Element>> inserted;

    // 元のlocationsに無いものをinsertedに保持。indexはまだ
    for (auto const &location : locations) {
        auto const contains = contains_if(this->_elements, [&location](auto const &prev) {
            return prev.has_value() && prev.value().identifier == location.identifier;
        });

        if (!contains) {
            inserted.emplace_back(0, location);
        }
    }

    std::vector<std::pair<std::size_t, Element>> erased;
    std::vector<std::size_t> null_indices;

    // 引数のlocationsに無いものを削除してerasedに保持。空のindexをnull_indicesに保持
    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &prev = this->_elements.at(idx);

        if (prev.has_value()) {
            auto const contains = contains_if(locations, [&prev_id = prev.value().identifier](auto const &location) {
                return location.identifier == prev_id;
            });

            if (!contains) {
                erased.emplace_back(idx, prev.value());
                this->_elements.at(idx) = std::nullopt;
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
            this->_elements.at(inserted_idx) = location_pair.second;
            inserted.at(idx) = std::make_pair(inserted_idx, location_pair.second);
        } else {
            this->_elements.emplace_back(location_pair.second);
            inserted.at(idx) = std::make_pair(this->_elements.size() - 1, location_pair.second);
        }
    }

    // 変更があれば通知
    if (!inserted.empty() || !erased.empty()) {
        this->_fetcher->push({.type = recycle_pool_event_type::updated,
                              .elements = this->_elements,
                              .inserted = std::move(inserted),
                              .erased = std::move(erased)});
    }
}

template <typename Element>
void recycle_pool<Element>::erase(identifier const &erase_id) {
    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = this->_elements.at(idx);
        if (location.has_value() && location.value().identifier == erase_id) {
            std::vector<std::pair<std::size_t, Element>> erased{{idx, location.value()}};

            this->_elements.at(idx) = std::nullopt;

            this->_fetcher->push(
                {.type = recycle_pool_event_type::updated, .elements = this->_elements, .erased = std::move(erased)});
            break;
        }
    }
}

template <typename Element>
void recycle_pool<Element>::insert(Element const &location) {
    std::vector<std::pair<std::size_t, Element>> inserted;

    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &inserting = this->_elements.at(idx);
        if (inserting.has_value()) {
            if (inserting.value().identifier == location.identifier) {
                return;
            }
        } else {
            inserted.emplace_back(idx, location);
            this->_elements.at(idx) = location;
            break;
        }
    }

    if (inserted.empty()) {
        this->_elements.emplace_back(location);
        inserted.emplace_back(this->_elements.size() - 1, location);
    }

    this->_fetcher->push(
        {.type = recycle_pool_event_type::updated, .elements = this->_elements, .inserted = std::move(inserted)});
}

template <typename Element>
std::vector<std::optional<Element>> const &recycle_pool<Element>::elements() const {
    return this->_elements;
}

template <typename Element>
observing::syncable recycle_pool<Element>::observe_event(
    std::function<void(recycle_pool_event<Element> const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}
}  // namespace yas::ae
