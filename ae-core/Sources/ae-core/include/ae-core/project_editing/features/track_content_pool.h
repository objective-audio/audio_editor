//
//  track_content_pool.h
//

#pragma once

#include <ae-core/global/features/recycle_pool.h>

#include <ae-core/project_editing/value_types/track_content.hpp>

namespace yas::ae {
using track_content_pool_event_type = recycle_pool_event_type;
using track_content_pool_event = recycle_pool_event<track_content>;
using track_content_pool = recycle_pool<track_content>;
}  // namespace yas::ae
