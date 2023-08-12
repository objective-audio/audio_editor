//
//  ae_pasting_module_index.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

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
