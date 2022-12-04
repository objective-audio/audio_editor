//
//  ae_selected_marker_pool.hpp
//

#pragma once

#include <observing/yas_observing_umbrella.h>

#include <audio_editor_core/ae_selected_marker.hpp>
#include <map>

namespace yas::ae {
struct selected_marker_pool final {
    using marker_map = std::map<marker_index, selected_marker_object>;

    enum class event_type {
        fetched,
        inserted,
        erased,
    };

    struct event final {
        event_type type;
        marker_map markers;  // inserted, erased
    };

    selected_marker_pool();

    [[nodiscard]] marker_map const &markers() const;

    bool contains(marker_index const &) const;

    void toggle_marker(selected_marker_object const &);
    void insert_marker(selected_marker_object const &);
    void erase_marker(marker_index const &);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(event const &)> &&);

   private:
    marker_map _markers;

    observing::fetcher_ptr<event> _event_fetcher;
};
}  // namespace yas::ae
