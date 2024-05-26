//
//  marker.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <ae-core/project_editing/value_types/marker_index.hpp>

namespace yas::ae {
struct marker final {
    frame_index_t frame;
    std::string name;
};

struct marker_object final {
    object_id identifier;
    marker value;

    marker_object(object_id const &, marker &&);

    [[nodiscard]] marker_index index() const;
};
}  // namespace yas::ae
