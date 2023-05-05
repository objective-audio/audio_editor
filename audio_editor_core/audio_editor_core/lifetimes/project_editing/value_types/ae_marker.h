//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <audio_editor_core/ae_marker_index.hpp>

namespace yas::ae {
struct marker final {
    frame_index_t frame;
    std::string name;
};

struct marker_object final {
    object_id identifier;
    marker value;

    marker_object(object_id const &, marker &&);

    [[nodiscard]] marker_index index() const;
};
}  // namespace yas::ae
