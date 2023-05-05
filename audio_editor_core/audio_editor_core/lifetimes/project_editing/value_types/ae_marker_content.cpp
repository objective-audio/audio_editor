//
//  ae_marker_content.cpp
//

#include "ae_marker_content.h"

#include <audio_editor_core/ae_marker_content_utils.hpp>

using namespace yas;
using namespace yas::ae;

marker_content::marker_content(object_id const &identifier, frame_index_t const &frame, uint32_t const sample_rate,
                               float const scale, std::string const &name, bool const is_selected)
    : marker_content(identifier, marker_content_utils::to_x(frame, sample_rate, scale), name, is_selected) {
}

marker_content::marker_content(object_id const &identifier, float const x, std::string const &name,
                               bool const is_selected)
    : identifier(identifier), x(x), name(name), is_selected(is_selected) {
}

bool marker_content::operator==(marker_content const &rhs) const {
    return this->identifier == rhs.identifier && this->x == rhs.x && this->name == rhs.name &&
           this->is_selected == rhs.is_selected;
}

bool marker_content::operator!=(marker_content const &rhs) const {
    return !(*this == rhs);
}
