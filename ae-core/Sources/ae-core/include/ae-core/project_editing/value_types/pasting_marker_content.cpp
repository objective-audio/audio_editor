//
//  pasting_marker_content.cpp
//

#include "pasting_marker_content.hpp"

#include <ae-core/project_editing/value_types/marker_content_utils.hpp>
#include <ae-core/project_editing/value_types/pasting_marker.hpp>

using namespace yas;
using namespace yas::ae;

pasting_marker_content::pasting_marker_content(pasting_marker_object const &marker, uint32_t const sample_rate,
                                               float const scale)
    : pasting_marker_content(marker.identifier, marker_content_utils::to_x(marker.value.frame, sample_rate, scale)) {
}

pasting_marker_content::pasting_marker_content(yas::identifier const &identifier, float const x)
    : identifier(identifier), x(x) {
}

bool pasting_marker_content::operator==(pasting_marker_content const &rhs) const {
    return this->identifier == rhs.identifier && this->x == rhs.x;
}

bool pasting_marker_content::operator!=(pasting_marker_content const &rhs) const {
    return !(*this == rhs);
}
