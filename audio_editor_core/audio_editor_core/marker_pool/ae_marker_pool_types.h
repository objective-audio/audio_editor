//
//  ae_marker_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_marker.h>
#include <processing/yas_processing_common_types.h>

#include <map>

namespace yas::ae {
enum class marker_pool_event_type {
    any,
    inserted,
    erased,
};

struct marker_pool_event {
    marker_pool_event_type type;
    std::optional<ae::marker> marker;
    std::map<proc::frame_index_t, ae::marker> const &markers;
};
}  // namespace yas::ae
