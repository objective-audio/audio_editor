//
//  ae_file_module_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_file_track_types.h>

namespace yas::ae::file_module_utils {
[[nodiscard]] bool can_split_time_range(time::range const &, frame_index_t const);
}  // namespace yas::ae::file_module_utils
