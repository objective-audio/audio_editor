//
//  pasting_module_content_pool.h
//

#pragma once

#include <ae-core/global/features/recycle_pool.h>

#include <ae-core/project_editing/value_types/pasting_module_content.hpp>

namespace yas::ae {
using pasting_module_content_pool_event_type = recycle_pool_event_type;
using pasting_module_content_pool_event = recycle_pool_event<pasting_module_content>;
using pasting_module_content_pool = recycle_pool<pasting_module_content>;
}  // namespace yas::ae
