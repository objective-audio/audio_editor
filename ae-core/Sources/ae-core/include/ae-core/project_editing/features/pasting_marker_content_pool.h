//
//  pasting_marker_content_pool.h
//

#pragma once

#include <ae-core/global/features/recycle_pool.h>

#include <ae-core/project_editing/value_types/pasting_marker_content.hpp>

namespace yas::ae {
using pasting_marker_content_pool_event_type = recycle_pool_event_type;
using pasting_marker_content_pool_event = recycle_pool_event<pasting_marker_content>;
using pasting_marker_content_pool = recycle_pool<pasting_marker_content>;
}  // namespace yas::ae
