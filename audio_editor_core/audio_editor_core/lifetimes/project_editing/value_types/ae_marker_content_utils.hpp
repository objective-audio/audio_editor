//
//  ae_marker_content_utils.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae::marker_content_utils {
float to_x(frame_index_t const &frame, uint32_t const sample_rate, float const scale);
}
