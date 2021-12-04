//
//  ae_marker_location.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>

namespace yas::ae {
struct marker_location {
    identifier identifier;  // markerからコピーする
    float x;
};
}  // namespace yas::ae
