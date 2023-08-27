//
//  ae_selected_pool_private.h
//

#pragma once

namespace yas::ae {
template <typename Element, typename Index>
selected_pool<Element, Index>::selected_pool() {
    this->_event_fetcher =
        observing::fetcher<event>::make_shared([this] { return event{.type = selected_pool_event_type::fetched}; });
}

template <typename Element, typename Index>
typename selected_pool<Element, Index>::element_map const &selected_pool<Element, Index>::elements() const {
    return this->_elements;
}

template <typename Element, typename Index>
bool selected_pool<Element, Index>::contains(Index const &index) const {
    return this->_elements.contains(index);
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::begin_toggling() {
    this->_toggled = element_map{};
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::toggle(element_map &&toggled) {
    if (this->_toggled.has_value()) {
        auto const &previous = this->_toggled.value();

        element_map changed;

        // 前回から追加された要素
        for (auto const &pair : toggled) {
            if (previous.contains(pair.first)) {
                continue;
            }

            changed.emplace(pair);
        }

        // 前回から削除された要素
        for (auto const &pair : previous) {
            if (toggled.contains(pair.first)) {
                continue;
            }

            changed.emplace(pair);
        }

        for (auto const &pair : changed) {
            if (this->_elements.contains(pair.first)) {
                this->_elements.erase(pair.first);
            } else {
                this->_elements.emplace(pair);
            }
        }

        this->_toggled = std::move(toggled);

        if (!changed.empty()) {
            this->_event_fetcher->push({.type = selected_pool_event_type::toggled, .toggled = std::move(changed)});
        }
    }
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::end_toggling() {
    if (this->_toggled.has_value()) {
        this->_toggled.reset();
    }
}

template <typename Element, typename Index>
bool selected_pool<Element, Index>::can_clear() const {
    return !this->_elements.empty();
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::clear() {
    if (!this->can_clear()) {
        return;
    }

    auto erased = std::move(this->_elements);

    this->_elements.clear();
    this->_toggled.reset();

    this->_event_fetcher->push({.type = selected_pool_event_type::toggled, .toggled = std::move(erased)});
}

template <typename Element, typename Index>
observing::syncable selected_pool<Element, Index>::observe_event(std::function<void(event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
}  // namespace yas::ae
