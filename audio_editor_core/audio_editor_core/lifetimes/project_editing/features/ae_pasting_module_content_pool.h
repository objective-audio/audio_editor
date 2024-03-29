//
//  ae_pasting_module_content_pool.h
//

#pragma once

#include <audio_editor_core/ae_recycle_pool.h>

#include <audio_editor_core/ae_pasting_module_content.hpp>

namespace yas::ae {
using pasting_module_content_pool_event_type = recycle_pool_event_type;
using pasting_module_content_pool_event = recycle_pool_event<pasting_module_content>;
using pasting_module_content_pool = recycle_pool<pasting_module_content>;
}  // namespace yas::ae
