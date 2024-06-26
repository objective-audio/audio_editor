//
//  module_utils.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/project_editing/value_types/module_pool_types.h>

namespace yas::ae::module_utils {
[[nodiscard]] bool can_split_time_range(time::range const &, frame_index_t const);
}  // namespace yas::ae::module_utils
