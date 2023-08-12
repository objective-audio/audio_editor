//
//  ae_pasting_module_content.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>
#include <ui/yas_ui_types.h>

#include <audio_editor_core/ae_pasting_module_index.hpp>

namespace yas::ae {
class pasting_module_object;

struct pasting_module_content final {
    identifier identifier;
    track_index_t track;
    time::range range;
    uint32_t sample_rate;
    float scale;

    pasting_module_content(pasting_module_object const &, uint32_t const sample_rate, float const scale);
    pasting_module_content(yas::identifier const &, track_index_t const, time::range const &,
                           uint32_t const sample_rate, float const scale);

    [[nodiscard]] pasting_module_index index() const;

    [[nodiscard]] ui::region region() const;

    bool operator==(pasting_module_content const &rhs) const;
    bool operator!=(pasting_module_content const &rhs) const;
};
}  // namespace yas::ae
