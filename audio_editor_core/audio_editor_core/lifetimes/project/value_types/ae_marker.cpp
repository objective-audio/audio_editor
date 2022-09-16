//
//  ae_marker.cpp
//

#include "ae_marker.h"

using namespace yas;
using namespace yas::ae;

marker::marker(object_id const &identifier, params &&params) : marker(identifier, params.frame, params.name) {
}

marker::marker(object_id const &identifier, frame_index_t const frame, std::string const &name)
    : identifier(identifier), frame(frame), name(name) {
}

marker::params marker::parameters() const {
    return {this->frame, this->name};
}
