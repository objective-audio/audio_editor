//
//  ae_selected_pool_private.h
//

#pragma once

namespace yas::ae {
template <typename Element>
selected_pool<Element>::selected_pool() {
    this->_event_fetcher =
        observing::fetcher<event>::make_shared([this] { return event{.type = selected_pool_event_type::fetched}; });
}

template <typename Element>
typename selected_pool<Element>::element_set const &selected_pool<Element>::elements() const {
    return this->_elements;
}

template <typename Element>
bool selected_pool<Element>::contains(Element const &element) const {
    return this->_elements.contains(element);
}

template <typename Element>
void selected_pool<Element>::begin_toggling() {
    this->_toggled = element_set{};
}

template <typename Element>
void selected_pool<Element>::toggle(element_set const &toggled) {
    if (this->_toggled.has_value()) {
        auto const &previous = this->_toggled.value();

        element_set changed;

        // 前回から追加された要素
        for (auto const &element : toggled) {
            if (previous.contains(element)) {
                continue;
            }

            changed.emplace(element);
        }

        // 前回から削除された要素
        for (auto const &element : previous) {
            if (toggled.contains(element)) {
                continue;
            }

            changed.emplace(element);
        }

        for (auto const &index : changed) {
            if (this->_elements.contains(index)) {
                this->_elements.erase(index);
            } else {
                this->_elements.emplace(index);
            }
        }

        this->_toggled = toggled;

        if (!changed.empty()) {
            this->_event_fetcher->push({.type = selected_pool_event_type::toggled, .toggled = std::move(changed)});
        }
    }
}

template <typename Element>
void selected_pool<Element>::end_toggling() {
    if (this->_toggled.has_value()) {
        this->_toggled.reset();
    }
}

template <typename Element>
bool selected_pool<Element>::can_clear() const {
    return !this->_elements.empty();
}

template <typename Element>
void selected_pool<Element>::clear() {
    if (!this->can_clear()) {
        return;
    }

    auto erased = std::move(this->_elements);

    this->_elements.clear();
    this->_toggled.reset();

    this->_event_fetcher->push({.type = selected_pool_event_type::toggled, .toggled = std::move(erased)});
}

template <typename Element>
observing::syncable selected_pool<Element>::observe_event(std::function<void(event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
}  // namespace yas::ae
