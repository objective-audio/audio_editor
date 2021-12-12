//
//  ae_module_location_pool.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_recycle_pool.h>

namespace yas::ae {
using module_location_pool_event_type = recycle_pool_event_type;
using module_location_pool_event = recycle_pool_event<module_location>;
using module_location_pool = recycle_pool<module_location>;
}  // namespace yas::ae
