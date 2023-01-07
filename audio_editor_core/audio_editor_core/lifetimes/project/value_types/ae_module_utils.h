//
//  ae_module_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_module_pool_types.h>

namespace yas::ae::module_utils {
[[nodiscard]] bool can_split_time_range(time::range const &, frame_index_t const);
}  // namespace yas::ae::module_utils
