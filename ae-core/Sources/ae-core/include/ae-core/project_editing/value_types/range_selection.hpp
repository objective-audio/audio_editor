//
//  range_selection.hpp
//

#pragma once

#include <ae-core/project_editing/value_types/point_range.hpp>

namespace yas::ae {
enum range_selection_phase {
    began,
    moved,
    ended,
};

struct range_selection {
    range_selection_phase phase;
    std::optional<point_range> range;

    bool operator==(range_selection const &rhs) const;
    bool operator!=(range_selection const &rhs) const;
};
}  // namespace yas::ae
