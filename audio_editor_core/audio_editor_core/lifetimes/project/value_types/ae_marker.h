//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct marker {
    frame_index_t frame;
    std::string name;
};

struct marker_object final {
    object_id identifier;
    frame_index_t frame;
    std::string name;

    marker_object(object_id const &, marker &&);
    marker_object(object_id const &, frame_index_t const frame, std::string const &name);
};
}  // namespace yas::ae
