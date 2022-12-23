//
//  ae_marker_content_utils.cpp
//

#include "ae_marker_content_utils.hpp"

using namespace yas;
using namespace yas::ae;

float marker_content_utils::to_x(frame_index_t const &frame, uint32_t const sample_rate, float const scale) {
    double const position = static_cast<double>(frame) / static_cast<double>(sample_rate);
    return static_cast<float>(position * scale);
}
