//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct marker final {
    object_id identifier;
    frame_index_t frame;
    std::string name;

    marker(object_id const &, frame_index_t const frame, std::string const &name);
};
}  // namespace yas::ae
