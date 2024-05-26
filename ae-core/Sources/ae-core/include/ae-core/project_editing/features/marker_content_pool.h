//
//  marker_content_pool.h
//

#pragma once

#include <ae-core/global/features/recycle_pool.h>
#include <ae-core/project_editing/value_types/marker_content.h>

namespace yas::ae {
using marker_content_pool_event_type = recycle_pool_event_type;
using marker_content_pool_event = recycle_pool_event<marker_content>;
using marker_content_pool = recycle_pool<marker_content>;
}  // namespace yas::ae
