//
//  ae_edge_holder_types.h
//

#pragma once

#include <ae-core/project_editing/value_types/ae_edge.h>

namespace yas::ae {
enum class edge_holder_event_type {
    fetched,
    updated,
    reverted,
};

struct edge_holder_event final {
    edge_holder_event_type type;
    ae::edge const &edge;
};
}  // namespace yas::ae
