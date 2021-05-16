//
//  ae_marker.h
//

#pragma once

#include <processing/yas_processing_common_types.h>

namespace yas::ae {
struct marker {
    proc::frame_index_t frame;

    bool operator==(marker const &rhs) const {
        return this->frame == rhs.frame;
    }

    bool operator!=(marker const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
