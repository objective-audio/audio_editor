//
//  pasting_marker_index.cpp
//

#include "pasting_marker_index.hpp"

using namespace yas;
using namespace yas::ae;

pasting_marker_index::pasting_marker_index(yas::identifier const &identifier, frame_index_t const &frame)
    : identifier(identifier), frame(frame) {
}

bool pasting_marker_index::operator==(pasting_marker_index const &rhs) const {
    return this->identifier == rhs.identifier;
}

bool pasting_marker_index::operator!=(pasting_marker_index const &rhs) const {
    return !(*this == rhs);
}

bool pasting_marker_index::operator<(pasting_marker_index const &rhs) const {
    if (this->frame == rhs.frame) {
        return this->identifier < rhs.identifier;
    } else {
        return this->frame < rhs.frame;
    }
}
