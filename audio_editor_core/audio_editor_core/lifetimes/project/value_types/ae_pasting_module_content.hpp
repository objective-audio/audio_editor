//
//  ae_pasting_module_content.hpp
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

#include <audio_editor_core/ae_pasting_file_module_index.hpp>

namespace yas::ae {
class pasting_file_module_object;

struct pasting_module_content final {
    identifier identifier;
    time::range range;
    uint32_t sample_rate;
    float scale;

    pasting_module_content(pasting_file_module_object const &, uint32_t const sample_rate, float const scale);
    pasting_module_content(yas::identifier const &, time::range const &, uint32_t const sample_rate, float const scale);

    [[nodiscard]] pasting_file_module_index index() const;

    [[nodiscard]] float x() const;
    [[nodiscard]] float width() const;

    bool operator==(pasting_module_content const &rhs) const;
    bool operator!=(pasting_module_content const &rhs) const;
};
}  // namespace yas::ae
