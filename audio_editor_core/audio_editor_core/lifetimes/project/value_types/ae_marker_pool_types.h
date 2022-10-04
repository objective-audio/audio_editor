//
//  ae_marker_pool_types.h
//

#pragma once

#include <audio_editor_core/ae_marker.h>

#include <map>

namespace yas::ae {
using marker_map_t = std::map<marker_index, ae::marker_object>;

static marker_map_t const empty_markers;

enum class marker_pool_event_type {
    any,
    reverted,
    inserted,
    erased,
    replaced,
};

struct marker_pool_event final {
    marker_pool_event_type type;
    std::optional<ae::marker_object> inserted{std::nullopt};
    std::optional<ae::marker_object> erased{std::nullopt};
    marker_map_t const &markers;
};
}  // namespace yas::ae
