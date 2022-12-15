//
//  ae_selected_marker_pool.hpp
//

#pragma once

#include <audio_editor_core/ae_selected_marker_pool_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct selected_marker_pool final {
    enum class event_type {
        fetched,
        inserted,
        erased,
    };

    struct event final {
        event_type type;
        selected_marker_map markers;  // inserted, erased
    };

    selected_marker_pool();

    [[nodiscard]] selected_marker_map const &markers() const;

    bool contains(marker_index const &) const;

    void toggle_marker(selected_marker_object const &);
    void insert_marker(selected_marker_object const &);
    void erase_marker(marker_index const &);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    selected_marker_map _markers;

    observing::fetcher_ptr<event> _event_fetcher;

    selected_marker_pool(selected_marker_pool const &) = delete;
    selected_marker_pool(selected_marker_pool &&) = delete;
    selected_marker_pool &operator=(selected_marker_pool const &) = delete;
    selected_marker_pool &operator=(selected_marker_pool &&) = delete;
};
}  // namespace yas::ae
