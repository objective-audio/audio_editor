//
//  edge_editor_dependencies.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/project_editing/value_types/edge.h>

namespace yas::ae {
struct database_for_edge_holder {
    virtual ~database_for_edge_holder() = default;

    virtual void set_edge(ae::edge const &) = 0;
};
}  // namespace yas::ae
