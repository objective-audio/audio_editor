//
//  ae_edge_editor_dependencies.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_edge.h>

namespace yas::ae {
struct database_for_edge_holder {
    virtual ~database_for_edge_holder() = default;

    virtual void set_edge(ae::edge const &) = 0;
};
}  // namespace yas::ae
