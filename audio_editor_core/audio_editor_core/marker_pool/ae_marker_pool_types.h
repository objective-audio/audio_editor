//
//  ae_marker_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_marker.h>
#include <processing/yas_processing_common_types.h>

#include <map>

namespace yas::ae {
using marker_map_t = std::map<proc::frame_index_t, ae::marker>;

static marker_map_t const empty_markers;

enum class marker_pool_event_type {
    any,
    reverted,
    inserted,
    erased,
};

struct marker_pool_event {
    marker_pool_event_type type;
    std::optional<ae::marker> marker{std::nullopt};
    marker_map_t const &markers;
};
}  // namespace yas::ae
