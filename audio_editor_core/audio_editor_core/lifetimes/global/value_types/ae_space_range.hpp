//
//  ae_space_range.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_track_range.hpp>

namespace yas::ae {
struct space_range {
    time::range time_range;
    track_range track_range;

    [[nodiscard]] bool operator==(space_range const &) const;
    [[nodiscard]] bool operator!=(space_range const &) const;
};
}  // namespace yas::ae
