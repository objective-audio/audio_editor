//
//  pasting_marker.cpp
//

#include "pasting_marker.hpp"

using namespace yas;
using namespace yas::ae;

pasting_marker::pasting_marker(frame_index_t const frame, std::string const &name) : frame(frame), name(name) {
}

pasting_marker_object::pasting_marker_object(yas::identifier const &identifier, pasting_marker &&value)
    : identifier(identifier), value(value) {
}
