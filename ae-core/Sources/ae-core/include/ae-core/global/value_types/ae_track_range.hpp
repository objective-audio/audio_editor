//
//  ae_track_range.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>

namespace yas::ae {
struct track_range final {
    track_index_t min;
    track_index_t max;

    [[nodiscard]] bool operator==(track_range const &) const;
    [[nodiscard]] bool operator!=(track_range const &) const;

    [[nodiscard]] bool contains(track_index_t const index) const;
};
}  // namespace yas::ae
