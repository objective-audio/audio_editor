//
//  ae_marker_location.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct marker_location final {
    identifier identifier;  // markerからコピーする
    float x;

    [[nodiscard]] static marker_location make_value(yas::identifier const &, frame_index_t const &frame,
                                                    uint32_t const sample_rate, float const scale);

    bool operator==(marker_location const &rhs) const;
    bool operator!=(marker_location const &rhs) const;
};
}  // namespace yas::ae
