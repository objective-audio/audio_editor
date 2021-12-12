//
//  ae_marker_location_pool.h
//

#pragma once

#include <audio_editor_core/ae_marker_location.h>
#include <audio_editor_core/ae_resource_pool.h>

namespace yas::ae {
using marker_location_pool_event_type = resource_pool_event_type;
using marker_location_pool_event = resource_pool_event<marker_location>;
using marker_location_pool = resource_pool<marker_location>;
}  // namespace yas::ae
