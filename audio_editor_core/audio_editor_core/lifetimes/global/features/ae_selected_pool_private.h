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
void selected_pool<Element, Index>::toggle(Element const &element) {
    auto const index = element.index();

    if (this->_elements.contains(index)) {
        this->erase(index);
    } else {
        this->insert(element);
    }
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::insert(Element const &element) {
    if (this->_elements.contains(element.index())) {
        return;
    }

    this->insert(std::vector<Element>{element});
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::insert(std::vector<Element> const &elements) {
    element_map inserted;

    for (auto const &element : elements) {
        auto index = element.index();

        if (this->_elements.contains(index)) {
            continue;
        }

        std::pair<Index, Element> pair{std::move(index), element};
        this->_elements.emplace(pair);
        inserted.emplace(std::move(pair));
    }

    if (!inserted.empty()) {
        this->_event_fetcher->push({.type = selected_pool_event_type::inserted, .elements = std::move(inserted)});
    }
}

template <typename Element, typename Index>
void selected_pool<Element, Index>::erase(Index const &index) {
    if (!this->_elements.contains(index)) {
        return;
    }

    auto const erasing = this->_elements.at(index);

    this->_elements.erase(index);

    this->_event_fetcher->push({.type = selected_pool_event_type::erased, .elements = {{index, erasing}}});
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

    this->_event_fetcher->push({.type = selected_pool_event_type::erased, .elements = std::move(erased)});
}

template <typename Element, typename Index>
observing::syncable selected_pool<Element, Index>::observe_event(std::function<void(event const &)> &&handler) {
    return this->_event_fetcher->observe(std::move(handler));
}
}  // namespace yas::ae
