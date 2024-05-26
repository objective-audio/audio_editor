//
//  pasting_marker_index.hpp
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <cpp-utils/identifier.h>

namespace yas::ae {
struct pasting_marker_index {
    identifier identifier;
    frame_index_t frame;

    pasting_marker_index(yas::identifier const &, frame_index_t const &);

    bool operator==(pasting_marker_index const &) const;
    bool operator!=(pasting_marker_index const &) const;
    bool operator<(pasting_marker_index const &) const;
};
}  // namespace yas::ae
