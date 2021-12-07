//
//  ae_module_location_pool.cpp
//

#include "ae_module_location_pool.h"

#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<module_location_pool> module_location_pool::make_shared() {
    return std::shared_ptr<module_location_pool>(new module_location_pool{});
}

module_location_pool::module_location_pool() {
    this->_fetcher = observing::fetcher<module_location_pool_event>::make_shared([this] {
        return module_location_pool_event{.type = module_location_pool_event_type::fetched,
                                          .locations = this->_locations};
    });
}

void module_location_pool::replace_all(std::vector<module_location> const &locations) {
    this->_locations.clear();
    this->_locations =
        to_vector<std::optional<module_location>>(locations, [](auto const &location) { return location; });

    this->_fetcher->push({.type = module_location_pool_event_type::replaced, .locations = this->_locations});
}

void module_location_pool::update_all(std::vector<module_location> const &locations) {
    std::vector<std::pair<std::size_t, module_location>> inserted;

    // 元のlocationsに無いものをinsertedに保持。indexはまだ
    for (auto const &location : locations) {
        bool exists = false;
        for (auto const &prev : this->_locations) {
            if (prev.has_value()) {
                if (prev.value().identifier == location.identifier) {
                    exists = true;
                    break;
                }
            }
        }

        if (!exists) {
            inserted.emplace_back(0, location);
        }
    }

    std::vector<std::pair<std::size_t, module_location>> erased;
    std::vector<std::size_t> null_indices;

    // 引数のlocationsに無いものを削除してerasedに保持。空のindexをnull_indicesに保持
    auto each = make_fast_each(this->_locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &prev = this->_locations.at(idx);

        if (prev.has_value()) {
            auto const &prev_id = prev.value().identifier;
            bool exists = false;
            for (auto const &location : locations) {
                if (location.identifier == prev_id) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
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
        this->_fetcher->push({.type = module_location_pool_event_type::updated,
                              .locations = this->_locations,
                              .inserted = std::move(inserted),
                              .erased = std::move(erased)});
    }
}

void module_location_pool::erase(identifier const &erase_id) {
    auto each = make_fast_each(this->_locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto &location = this->_locations.at(idx);
        if (location.has_value() && location.value().identifier == erase_id) {
            std::vector<std::pair<std::size_t, module_location>> erased;
            erased.emplace_back(idx, location.value());

            location = std::nullopt;

            this->_fetcher->push({.type = module_location_pool_event_type::updated,
                                  .locations = this->_locations,
                                  .erased = std::move(erased)});
            break;
        }
    }
}

void module_location_pool::insert(module_location const &location) {
    std::vector<std::pair<std::size_t, module_location>> inserted;

    auto each = make_fast_each(this->_locations.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto &inserting = this->_locations.at(idx);
        if (inserting.has_value()) {
            if (inserting.value().identifier == location.identifier) {
                return;
            }
        } else {
            inserted.emplace_back(idx, location);
            inserting = location;
            break;
        }
    }

    if (inserted.empty()) {
        this->_locations.emplace_back(location);
        inserted.emplace_back(this->_locations.size() - 1, location);
    }

    this->_fetcher->push({.type = module_location_pool_event_type::updated,
                          .locations = this->_locations,
                          .inserted = std::move(inserted)});
}

std::vector<std::optional<module_location>> const &module_location_pool::locations() const {
    return this->_locations;
}

observing::syncable module_location_pool::observe_event(
    std::function<void(module_location_pool_event const &)> &&handler) {
    return this->_fetcher->observe(std::move(handler));
}
