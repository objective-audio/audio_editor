//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct marker final {
    struct params {
        frame_index_t frame;
        std::string name;
    };

    object_id identifier;
    frame_index_t frame;
    std::string name;

    marker(object_id const &, params &&);
    marker(object_id const &, frame_index_t const frame, std::string const &name);

    [[nodiscard]] params parameters() const;
};
}  // namespace yas::ae
