//
//  ae_marker_content_pool.h
//

#pragma once

#include <audio_editor_core/ae_marker_content.h>
#include <audio_editor_core/ae_recycle_pool.h>

namespace yas::ae {
using marker_content_pool_event_type = recycle_pool_event_type;
using marker_content_pool_event = recycle_pool_event<marker_content>;
using marker_content_pool = recycle_pool<marker_content>;
}  // namespace yas::ae
