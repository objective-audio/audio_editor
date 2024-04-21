//
//  ae_selected_pool.hpp
//

#pragma once

#include <observing/umbrella.hpp>
#include <set>

namespace yas::ae {
enum class selected_pool_event_type {
    fetched,
    replaced,
    toggled,
};

template <typename Element>
struct selected_pool final {
    using element_set = std::set<Element>;

    struct event final {
        selected_pool_event_type type;
        element_set toggled;
    };

    selected_pool();

    [[nodiscard]] element_set const &elements() const;

    [[nodiscard]] bool contains(Element const &) const;

    void begin_toggling();
    void toggle(element_set const &);
    void end_toggling();

    void replace(element_set &&);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    element_set _elements;
    std::optional<element_set> _toggled;

    observing::fetcher_ptr<event> _event_fetcher;

    selected_pool(selected_pool const &) = delete;
    selected_pool(selected_pool &&) = delete;
    selected_pool &operator=(selected_pool const &) = delete;
    selected_pool &operator=(selected_pool &&) = delete;
};
}  // namespace yas::ae

#include "ae_selected_pool_private.h"
