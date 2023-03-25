//
//  ae_grid_content.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

namespace yas::ae {
struct grid_content final {
    frame_index_t identifier;  // frameと同じ
    frame_index_t frame;
    uint32_t sample_rate;

    grid_content(frame_index_t const frame, uint32_t const sample_rate);

    [[nodiscard]] float x() const;

    bool operator==(grid_content const &rhs) const;
    bool operator!=(grid_content const &rhs) const;
};
}  // namespace yas::ae
