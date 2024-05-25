//
//  space_range.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <ae-core/global/value_types/track_range.hpp>

namespace yas::ae {
struct space_range {
    time::range time_range;
    track_range track_range;

    [[nodiscard]] bool operator==(space_range const &) const;
    [[nodiscard]] bool operator!=(space_range const &) const;
};
}  // namespace yas::ae
