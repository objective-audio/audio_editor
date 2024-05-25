//
//  marker_content.cpp
//

#include "marker_content.h"

#include <ae-core/project_editing/value_types/marker_content_utils.hpp>

using namespace yas;
using namespace yas::ae;

marker_content::marker_content(object_id const &identifier, frame_index_t const &frame, uint32_t const sample_rate,
                               float const scale, std::string const &name, bool const is_selected)
    : identifier(identifier),
      frame(frame),
      sample_rate(sample_rate),
      scale(scale),
      name(name),
      is_selected(is_selected) {
}

marker_index marker_content::marker_index() const {
    return {this->identifier, this->frame};
}

float marker_content::x() const {
    return marker_content_utils::to_x(frame, sample_rate, scale);
}

bool marker_content::operator==(marker_content const &rhs) const {
    return this->identifier == rhs.identifier && this->frame == rhs.frame && this->sample_rate == rhs.sample_rate &&
           this->scale == rhs.scale && this->name == rhs.name && this->is_selected == rhs.is_selected;
}

bool marker_content::operator!=(marker_content const &rhs) const {
    return !(*this == rhs);
}
