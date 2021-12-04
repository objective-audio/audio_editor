//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <processing/yas_processing_common_types.h>

namespace yas::ae {
struct marker {
    identifier identifier;
    proc::frame_index_t frame;

    bool operator==(marker const &rhs) const {
        return this->frame == rhs.frame;
    }

    bool operator!=(marker const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
