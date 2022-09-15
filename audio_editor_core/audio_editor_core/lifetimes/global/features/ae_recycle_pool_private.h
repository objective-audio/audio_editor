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
void recycle_pool<Element>::replace_all(std::vector<Element> const &elements) {
    this->_elements.clear();
    this->_elements = map<std::optional<Element>>(elements, [](auto const &element) { return element; });

    this->_fetcher->push({.type = recycle_pool_event_type::replaced, .elements = this->_elements});
}

template <typename Element>
void recycle_pool<Element>::update_all(std::vector<Element> const &elements, bool const force_replacing) {
    std::vector<std::pair<std::size_t, Element>> inserted;
    std::vector<std::pair<std::size_t, Element>> replaced;

    // 元のelementsに無いものをinsertedに保持。indexはまだ
    for (auto const &element : elements) {
        auto const contains_index = index(this->_elements, [&element](auto const &prev) {
            return prev.has_value() && prev.value().identifier == element.identifier;
        });

        if (contains_index.has_value()) {
            auto const &idx = contains_index.value();
            if (force_replacing || this->_elements.at(idx) != element) {
                replaced.emplace_back(idx, element);
                this->_elements.at(idx) = element;
            }
        } else {
            inserted.emplace_back(0, element);
        }
    }

    std::vector<std::pair<std::size_t, Element>> erased;
    std::vector<std::size_t> null_indices;

    // 引数のelementsに無いものを削除してerasedに保持。空のindexをnull_indicesに保持
    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &prev = this->_elements.at(idx);

        if (prev.has_value()) {
            auto const contains = contains_if(elements, [&prev_id = prev.value().identifier](auto const &element) {
                return element.identifier == prev_id;
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
        auto const &element_pair = inserted.at(idx);

        if (idx < null_indices.size()) {
            auto const inserted_idx = null_indices.at(idx);
            this->_elements.at(inserted_idx) = element_pair.second;
            inserted.at(idx) = std::make_pair(inserted_idx, element_pair.second);
        } else {
            this->_elements.emplace_back(element_pair.second);
            inserted.at(idx) = std::make_pair(this->_elements.size() - 1, element_pair.second);
        }
    }

    // 変更があれば通知
    if (!inserted.empty() || !replaced.empty() || !erased.empty()) {
        this->_fetcher->push({.type = recycle_pool_event_type::updated,
                              .elements = this->_elements,
                              .inserted = std::move(inserted),
                              .replaced = std::move(replaced),
                              .erased = std::move(erased)});
    }
}

template <typename Element>
void recycle_pool<Element>::erase(identifier const &erase_id) {
    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &element = this->_elements.at(idx);
        if (element.has_value() && element.value().identifier == erase_id) {
            std::vector<std::pair<std::size_t, Element>> erased{{idx, element.value()}};

            this->_elements.at(idx) = std::nullopt;

            this->_fetcher->push(
                {.type = recycle_pool_event_type::updated, .elements = this->_elements, .erased = std::move(erased)});
            break;
        }
    }
}

template <typename Element>
void recycle_pool<Element>::erase_for_id(object_id const &erase_id) {
    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &element = this->_elements.at(idx);
        if (element.has_value() && element.value().identifier == erase_id) {
            std::vector<std::pair<std::size_t, Element>> erased{{idx, element.value()}};

            this->_elements.at(idx) = std::nullopt;

            this->_fetcher->push(
                {.type = recycle_pool_event_type::updated, .elements = this->_elements, .erased = std::move(erased)});
            break;
        }
    }
}

template <typename Element>
void recycle_pool<Element>::insert(Element const &element) {
    std::vector<std::pair<std::size_t, Element>> inserted;

    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &inserting = this->_elements.at(idx);
        if (inserting.has_value()) {
            if (inserting.value().identifier == element.identifier) {
                return;
            }
        } else {
            inserted.emplace_back(idx, element);
            this->_elements.at(idx) = element;
            break;
        }
    }

    if (inserted.empty()) {
        this->_elements.emplace_back(element);
        inserted.emplace_back(this->_elements.size() - 1, element);
    }

    this->_fetcher->push(
        {.type = recycle_pool_event_type::updated, .elements = this->_elements, .inserted = std::move(inserted)});
}

template <typename Element>
void recycle_pool<Element>::replace(Element const &element) {
    std::vector<std::pair<std::size_t, Element>> replaced;

    auto each = make_fast_each(this->_elements.size());
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &overwriting = this->_elements.at(idx);
        if (overwriting.has_value()) {
            if (overwriting.value().identifier == element.identifier) {
                replaced.emplace_back(idx, element);
                this->_elements.at(idx) = element;
                break;
            }
        }
    }

    if (replaced.empty()) {
        return;
    }

    this->_fetcher->push(
        {.type = recycle_pool_event_type::updated, .elements = this->_elements, .replaced = std::move(replaced)});
}

template <typename Element>
void recycle_pool<Element>::insert_or_replace(Element const &element) {
    // 処理の重さを考慮していない。replaceする場合は2回vectorを走査してしまう
    if (contains_if(this->_elements, [&element](std::optional<Element> const &target) {
            return target.has_value() && target.value().identifier == element.identifier;
        })) {
        this->replace(element);
    } else {
        this->insert(element);
    }
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
