//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct marker final {
    frame_index_t frame;
    std::string name;
};

struct marker_object final {
    object_id identifier;
    marker value;

    marker_object(object_id const &, marker &&);
};
}  // namespace yas::ae
