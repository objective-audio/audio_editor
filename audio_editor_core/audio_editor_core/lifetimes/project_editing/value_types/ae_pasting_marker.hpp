//
//  ae_pasting_marker.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

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
