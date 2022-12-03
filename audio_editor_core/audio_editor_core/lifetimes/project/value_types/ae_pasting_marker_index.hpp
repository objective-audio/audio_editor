//
//  ae_pasting_marker_index.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

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
