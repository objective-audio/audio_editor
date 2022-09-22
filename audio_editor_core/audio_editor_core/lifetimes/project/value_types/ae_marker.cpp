//
//  ae_marker.cpp
//

#include "ae_marker.h"

using namespace yas;
using namespace yas::ae;

marker_object::marker_object(object_id const &identifier, marker &&value)
    : identifier(identifier), value(std::move(value)) {
}
