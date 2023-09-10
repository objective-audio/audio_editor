//
//  ae_track_content.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct track_content {
    track_index_t identifier;
    bool is_selected;

    track_index_t const &track() const;

    bool operator==(track_content const &rhs) const;
    bool operator!=(track_content const &rhs) const;
};
}  // namespace yas::ae
