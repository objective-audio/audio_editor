//
//  ae_grid_content_pool.h
//

#pragma once

#include <audio_editor_core/ae_recycle_pool.h>

#include <audio_editor_core/ae_grid_content.hpp>

namespace yas::ae {
using grid_content_pool_event_type = recycle_pool_event_type;
using grid_content_pool_event = recycle_pool_event<grid_content>;
using grid_content_pool = recycle_pool<grid_content>;
}  // namespace yas::ae
