//
//  module_index.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

namespace yas::ae {
class space_range;

struct module_index final {
    object_id object_id;
    time::range range;
    track_index_t track;

    module_index(ae::object_id const &, time::range const &, track_index_t const track);

    bool operator==(module_index const &) const;
    bool operator!=(module_index const &) const;
    bool operator<(module_index const &) const;

    [[nodiscard]] bool is_overlap(space_range const &) const;
};
}  // namespace yas::ae
