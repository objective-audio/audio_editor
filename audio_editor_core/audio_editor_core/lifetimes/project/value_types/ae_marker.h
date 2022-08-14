//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct marker final {
    identifier identifier;
    frame_index_t frame;
    std::string name;
};
}  // namespace yas::ae
