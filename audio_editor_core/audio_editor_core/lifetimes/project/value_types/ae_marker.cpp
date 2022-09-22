//
//  ae_marker.cpp
//

#include "ae_marker.h"

using namespace yas;
using namespace yas::ae;

marker_object::marker_object(object_id const &identifier, marker &&params)
    : marker_object(identifier, params.frame, params.name) {
}

marker_object::marker_object(object_id const &identifier, frame_index_t const frame, std::string const &name)
    : identifier(identifier), frame(frame), name(name) {
}

marker marker_object::parameters() const {
    return {this->frame, this->name};
}
