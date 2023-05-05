//
//  ae_module_content_pool.h
//

#pragma once

#include <audio_editor_core/ae_module_content.h>
#include <audio_editor_core/ae_recycle_pool.h>

namespace yas::ae {
using module_content_pool_event_type = recycle_pool_event_type;
using module_content_pool_event = recycle_pool_event<module_content>;
using module_content_pool = recycle_pool<module_content>;
}  // namespace yas::ae
