//
//  pasting_module_index.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <cpp-utils/identifier.h>

namespace yas::ae {
class space_range;

struct pasting_module_index final {
    identifier identifier;
    time::range range;
    track_index_t track;

    pasting_module_index(yas::identifier const &, time::range const &, track_index_t const);

    bool operator==(pasting_module_index const &) const;
    bool operator!=(pasting_module_index const &) const;
    bool operator<(pasting_module_index const &) const;

    [[nodiscard]] bool is_overlap(space_range const &) const;
};
}  // namespace yas::ae
