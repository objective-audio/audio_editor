//
//  ae_pasting_marker_content.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
class pasting_marker_object;

struct pasting_marker_content {
    identifier identifier;
    float x;

    pasting_marker_content(pasting_marker_object const &, uint32_t const sample_rate, float const scale);
    pasting_marker_content(yas::identifier const &, float const x);

    bool operator==(pasting_marker_content const &rhs) const;
    bool operator!=(pasting_marker_content const &rhs) const;
};
}  // namespace yas::ae
