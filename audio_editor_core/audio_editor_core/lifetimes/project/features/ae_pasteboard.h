//
//  ae_pasteboard.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

#include <audio_editor_core/ae_pasting_file_module.hpp>
#include <audio_editor_core/ae_pasting_marker.hpp>

namespace yas::ae {
enum class pasting_kind {
    file_module,
    marker,
};

enum class pasteboard_event {
    fetched,
    file_modules_replaced,
    file_modules_cleared,
    markers_replaced,
    markers_cleared,
};

struct pasteboard final {
    pasteboard();

    [[nodiscard]] bool has_file_modules() const;
    [[nodiscard]] std::vector<pasting_file_module_object> const &file_modules() const;
    void set_file_modules(std::vector<pasting_file_module_object> const &);

    [[nodiscard]] bool has_markers() const;
    [[nodiscard]] std::vector<pasting_marker_object> const &markers() const;
    void set_markers(std::vector<pasting_marker_object> const &);

    [[nodiscard]] bool can_clear() const;
    void clear();

    [[nodiscard]] observing::syncable observe_event(std::function<void(pasteboard_event const &)> &&);

   private:
    std::vector<pasting_file_module_object> _modules;
    std::vector<pasting_marker_object> _markers;

    observing::fetcher_ptr<pasteboard_event> const _event_fetcher;

    pasteboard(pasteboard const &) = delete;
    pasteboard(pasteboard &&) = delete;
    pasteboard &operator=(pasteboard const &) = delete;
    pasteboard &operator=(pasteboard &&) = delete;
};
}  // namespace yas::ae
