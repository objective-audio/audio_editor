//
//  ae_display_space_track_range.hpp
//

#pragma once

#include <audio_editor_core/ae_track_range.hpp>
#include <optional>

namespace yas::ae {
class track_range;
class display_space;
class vertical_scrolling;

struct display_space_track_range final {
    display_space_track_range(display_space const *, vertical_scrolling const *);

    [[nodiscard]] std::optional<track_range> current() const;
    [[nodiscard]] std::optional<track_range> zero() const;

   private:
    display_space const *const _display_space;
    vertical_scrolling const *const _scrolling;

    display_space_track_range(display_space_track_range const &) = delete;
    display_space_track_range(display_space_track_range &&) = delete;
    display_space_track_range &operator=(display_space_track_range const &) = delete;
    display_space_track_range &operator=(display_space_track_range &&) = delete;
};
}  // namespace yas::ae
