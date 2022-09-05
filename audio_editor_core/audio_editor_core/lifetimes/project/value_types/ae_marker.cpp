//
//  ae_marker.cpp
//

#include "ae_marker.h"

using namespace yas;
using namespace yas::ae;

marker::marker(yas::identifier const &identifier, frame_index_t const frame, std::string const &name)
    : identifier(identifier), frame(frame), name(name) {
}
