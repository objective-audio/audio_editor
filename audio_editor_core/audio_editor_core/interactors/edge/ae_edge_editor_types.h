//
//  ae_edge_editor_types.h
//

#pragma once

#include <audio_editor_core/ae_edge.h>

namespace yas::ae {
enum class edge_editor_event_type {
    fetched,
    updated,
    reverted,
};

struct edge_editor_event final {
    edge_editor_event_type type;
    ae::edge const &edge;
};
}  // namespace yas::ae
