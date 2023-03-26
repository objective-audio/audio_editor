//
//  ae_selected_pool.hpp
//

#pragma once

#include <observing/yas_observing_umbrella.h>

#include <map>

namespace yas::ae {
enum class selected_pool_event_type {
    fetched,
    inserted,
    erased,
};

template <typename Element, typename Index>
struct selected_pool final {
    using element_map = std::map<Index, Element>;

    struct event final {
        selected_pool_event_type type;
        element_map elements;  // inserted, erased
    };

    selected_pool();

    [[nodiscard]] element_map const &elements() const;

    [[nodiscard]] bool contains(Index const &) const;

    void toggle(Element const &);
    void insert(Element const &);
    void insert(std::vector<Element> const &);
    void erase(Index const &);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    element_map _elements;

    observing::fetcher_ptr<event> _event_fetcher;

    selected_pool(selected_pool const &) = delete;
    selected_pool(selected_pool &&) = delete;
    selected_pool &operator=(selected_pool const &) = delete;
    selected_pool &operator=(selected_pool &&) = delete;
};
}  // namespace yas::ae

#include <audio_editor_core/ae_selected_pool_private.h>
