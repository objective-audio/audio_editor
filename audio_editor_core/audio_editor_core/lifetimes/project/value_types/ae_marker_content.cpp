//
//  ae_marker_content.cpp
//

#include "ae_marker_content.h"

using namespace yas;
using namespace yas::ae;

namespace yas::ae::marker_content_utils {
static float to_x(frame_index_t const &frame, uint32_t const sample_rate, float const scale) {
    double const position = static_cast<double>(frame) / static_cast<double>(sample_rate);
    return static_cast<float>(position * scale);
}
}  // namespace yas::ae::marker_content_utils

marker_content::marker_content(object_id const &identifier, frame_index_t const &frame, uint32_t const sample_rate,
                               float const scale, std::string const &name, bool const is_selected)
    : marker_content(identifier, marker_content_utils::to_x(frame, sample_rate, scale), name, is_selected) {
}

marker_content::marker_content(object_id const &identifier, float const x, std::string const &name,
                               bool const is_selected)
    : identifier(identifier), x(x), name(name), is_selected(is_selected) {
}

bool marker_content::operator==(marker_content const &rhs) const {
    return this->identifier == rhs.identifier && this->x == rhs.x && this->is_selected == rhs.is_selected;
}

bool marker_content::operator!=(marker_content const &rhs) const {
    return !(*this == rhs);
}
