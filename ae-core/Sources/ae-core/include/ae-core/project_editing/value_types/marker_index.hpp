//
//  marker_index.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

namespace yas::ae {
struct marker_index final {
    object_id object_id;
    frame_index_t frame;

    marker_index(ae::object_id const &, frame_index_t const);

    bool operator==(marker_index const &) const;
    bool operator!=(marker_index const &) const;
    bool operator<(marker_index const &) const;
};
}  // namespace yas::ae
