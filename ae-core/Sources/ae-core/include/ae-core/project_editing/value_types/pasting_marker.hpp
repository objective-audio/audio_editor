//
//  pasting_marker.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <cpp-utils/identifier.h>

namespace yas::ae {
struct pasting_marker {
    frame_index_t frame;
    std::string name;

    pasting_marker(frame_index_t const frame, std::string const &name);
};

struct pasting_marker_object {
    identifier identifier;
    pasting_marker value;

    pasting_marker_object(yas::identifier const &, pasting_marker &&);
};
}  // namespace yas::ae
