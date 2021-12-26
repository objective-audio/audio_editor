//
//  ae_marker_location.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <processing/yas_processing_common_types.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct marker_location {
    identifier identifier;  // markerからコピーする
    ui::point point;
    float x;

    [[nodiscard]] static marker_location make_value(ae::identifier const &, proc::frame_index_t const &frame,
                                                    uint32_t const sample_rate, ui::size const &scale);

    bool operator==(marker_location const &rhs) const {
        return this->identifier == rhs.identifier && this->point == rhs.point && this->x == rhs.x;
    }

    bool operator!=(marker_location const &rhs) const {
        return !(*this == rhs);
    }
};
}  // namespace yas::ae
