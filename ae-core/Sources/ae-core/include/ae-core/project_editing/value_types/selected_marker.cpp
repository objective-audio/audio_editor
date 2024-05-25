//
//  selected_marker.cpp
//

#include "selected_marker.hpp"

using namespace yas;
using namespace yas::ae;

selected_marker::selected_marker(frame_index_t const frame) : frame(frame) {
}

selected_marker_object::selected_marker_object(object_id const &identifier, selected_marker const &value)
    : identifier(identifier), value(value) {
}

marker_index selected_marker_object::index() const {
    return {this->identifier, this->value.frame};
}

std::string yas::to_string(ae::selected_marker const &marker) {
    return "{frame:" + std::to_string(marker.frame) + "}";
}

std::ostream &operator<<(std::ostream &os, yas::ae::selected_marker const &value) {
    os << to_string(value);
    return os;
}
