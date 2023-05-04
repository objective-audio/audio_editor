//
//  ae_display_space_range.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_lifetime_id.h>

namespace yas::ae {
class display_space;
class player;

struct display_space_range final {
    display_space_range(project_format const, display_space const *, player const *);

    [[nodiscard]] std::optional<time::range> current() const;
    [[nodiscard]] std::optional<time::range> zero() const;

   private:
    project_format const _project_format;
    display_space const *const _display_space;
    player const *const _player;

    display_space_range(display_space_range const &) = delete;
    display_space_range(display_space_range &&) = delete;
    display_space_range &operator=(display_space_range const &) = delete;
    display_space_range &operator=(display_space_range &&) = delete;

    std::optional<time::range> _range(frame_index_t const frame) const;
};
}  // namespace yas::ae
