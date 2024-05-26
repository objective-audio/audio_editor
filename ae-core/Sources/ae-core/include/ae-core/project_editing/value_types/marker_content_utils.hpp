//
//  marker_content_utils.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

namespace yas::ae::marker_content_utils {
float to_x(frame_index_t const &frame, uint32_t const sample_rate, float const scale);
}
