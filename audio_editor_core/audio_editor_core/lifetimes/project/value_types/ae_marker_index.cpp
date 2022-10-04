//
//  ae_marker_index.cpp
//

#include "ae_marker_index.hpp"

using namespace yas;
using namespace yas::ae;

marker_index::marker_index(ae::object_id const &object_id, frame_index_t const frame)
    : object_id(object_id), frame(frame) {
}

bool marker_index::operator==(marker_index const &rhs) const {
    return this->object_id == rhs.object_id;
}

bool marker_index::operator!=(marker_index const &rhs) const {
    return !(*this == rhs);
}

bool marker_index::operator<(marker_index const &rhs) const {
    if (this->frame == rhs.frame) {
        return this->object_id.identifier() < rhs.object_id.identifier();
    } else {
        return this->frame < rhs.frame;
    }
}
