//
//  display_space_range.hpp
//

#pragma once

#include <optional>

namespace yas::ae {
class space_range;
class display_space_time_range;
class display_space_track_range;

struct display_space_range final {
    display_space_range(display_space_time_range *, display_space_track_range *);

    [[nodiscard]] std::optional<space_range> current() const;
    [[nodiscard]] std::optional<space_range> zero() const;

   private:
    display_space_time_range *const _time_range;
    display_space_track_range *const _track_range;

    display_space_range(display_space_range const &) = delete;
    display_space_range(display_space_range &&) = delete;
    display_space_range &operator=(display_space_range const &) = delete;
    display_space_range &operator=(display_space_range &&) = delete;
};
}  // namespace yas::ae
