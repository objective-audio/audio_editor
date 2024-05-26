//
//  pasteboard.h
//

#pragma once

#include <ae-core/project_editing/value_types/pasting_marker.hpp>
#include <ae-core/project_editing/value_types/pasting_module.hpp>
#include <observing/umbrella.hpp>

namespace yas::ae {
enum class pasting_kind {
    module,
    marker,
};

enum class pasteboard_event {
    fetched,
    modules_replaced,
    modules_cleared,
    markers_replaced,
    markers_cleared,
};

struct pasteboard final {
    pasteboard();

    [[nodiscard]] bool has_modules() const;
    [[nodiscard]] std::vector<pasting_module_object> const &modules() const;
    void set_modules(std::vector<pasting_module_object> const &);

    [[nodiscard]] bool has_markers() const;
    [[nodiscard]] std::vector<pasting_marker_object> const &markers() const;
    void set_markers(std::vector<pasting_marker_object> const &);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&);

   private:
    std::vector<pasting_module_object> _modules;
    std::vector<pasting_marker_object> _markers;

    observing::fetcher_ptr<pasteboard_event> const _event_fetcher;

    pasteboard(pasteboard const &) = delete;
    pasteboard(pasteboard &&) = delete;
    pasteboard &operator=(pasteboard const &) = delete;
    pasteboard &operator=(pasteboard &&) = delete;
};
}  // namespace yas::ae
