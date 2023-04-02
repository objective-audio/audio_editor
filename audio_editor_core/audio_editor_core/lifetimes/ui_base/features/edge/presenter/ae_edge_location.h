//
//  ae_edge_location.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct edge_location final {
    float x;

    edge_location(frame_index_t const &frame, uint32_t const sample_rate, ui::size const &scale);
    edge_location(float const x);

    bool operator==(edge_location const &) const;
    bool operator!=(edge_location const &) const;
};

struct edge_locations final {
    edge_location begin;
    edge_location end;

    bool operator==(edge_locations const &) const;
    bool operator!=(edge_locations const &) const;
};
}  // namespace yas::ae
