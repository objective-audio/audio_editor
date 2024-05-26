//
//  display_space_time_range.hpp
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/common_types.h>
#include <ae-core/global/value_types/project_lifetime_id.h>

namespace yas::ae {
class display_space;
class player;

struct display_space_time_range final {
    display_space_time_range(project_format const, display_space const *, player const *);

    [[nodiscard]] std::optional<time::range> current() const;
    [[nodiscard]] std::optional<time::range> zero() const;

   private:
    project_format const _project_format;
    display_space const *const _display_space;
    player const *const _player;

    display_space_time_range(display_space_time_range const &) = delete;
    display_space_time_range(display_space_time_range &&) = delete;
    display_space_time_range &operator=(display_space_time_range const &) = delete;
    display_space_time_range &operator=(display_space_time_range &&) = delete;

    std::optional<time::range> _range(frame_index_t const frame) const;
};
}  // namespace yas::ae
