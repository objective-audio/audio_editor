//
//  ae_marker.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_identifier.h>

namespace yas::ae {
struct marker final {
    identifier identifier;
    frame_index_t frame;
};
}  // namespace yas::ae
