//
//  marker.cpp
//

#include "marker.h"

using namespace yas;
using namespace yas::ae;

marker_object::marker_object(object_id const &identifier, marker &&value)
    : identifier(identifier), value(std::move(value)) {
}

marker_index marker_object::index() const {
    return {this->identifier, this->value.frame};
}
