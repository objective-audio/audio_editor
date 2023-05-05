//
//  ae_marker_content.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct marker_content final {
    object_id identifier;  // markerからコピーする
    float x;
    std::string name;
    bool is_selected;

    marker_content(object_id const &, frame_index_t const &frame, uint32_t const sample_rate, float const scale,
                   std::string const &name, bool const is_selected);
    marker_content(object_id const &, float const x, std::string const &name, bool const is_selected);

    bool operator==(marker_content const &rhs) const;
    bool operator!=(marker_content const &rhs) const;
};
}  // namespace yas::ae
