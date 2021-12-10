//
//  ae_module_location_pool.h
//

#pragma once

#include <audio_editor_core/ae_module_location.h>
#include <audio_editor_core/ae_resource_pool.h>

namespace yas::ae {
using module_location_pool_event_type = resource_pool_event_type;
using module_location_pool_event = resource_pool_event<module_location>;
using module_location_pool = resource_pool<module_location>;
}  // namespace yas::ae
