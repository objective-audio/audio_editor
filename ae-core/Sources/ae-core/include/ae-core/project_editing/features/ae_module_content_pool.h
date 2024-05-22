//
//  ae_module_content_pool.h
//

#pragma once

#include <ae-core/global/features/ae_recycle_pool.h>
#include <ae-core/project_editing/value_types/ae_module_content.h>

namespace yas::ae {
using module_content_pool_event_type = recycle_pool_event_type;
using module_content_pool_event = recycle_pool_event<module_content>;
using module_content_pool = recycle_pool<module_content>;
}  // namespace yas::ae
