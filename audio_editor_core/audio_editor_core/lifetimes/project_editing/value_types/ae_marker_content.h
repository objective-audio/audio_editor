//
//  ae_marker_content.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <ui/yas_ui_types.h>

#include <audio_editor_core/ae_marker_index.hpp>

namespace yas::ae {
struct marker_content final {
    object_id identifier;  // markerからコピーする
    frame_index_t frame;
    uint32_t sample_rate;
    float scale;
    std::string name;
    bool is_selected;

    marker_content(object_id const &, frame_index_t const &frame, uint32_t const sample_rate, float const scale,
                   std::string const &name, bool const is_selected);

    [[nodiscard]] marker_index marker_index() const;

    [[nodiscard]] float x() const;

    bool operator==(marker_content const &rhs) const;
    bool operator!=(marker_content const &rhs) const;
};
}  // namespace yas::ae
