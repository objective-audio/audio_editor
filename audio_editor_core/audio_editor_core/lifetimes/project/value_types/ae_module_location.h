//
//  ae_module_location.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
class file_module;

struct module_location final {
    struct mesh_element final {
        static uint32_t const max_length = 512;

        uint32_t rect_count;
        time::range range;  // module_location内の位置

        bool operator==(mesh_element const &rhs) const;
        bool operator!=(mesh_element const &rhs) const;
    };

    module_location(file_module const &, uint32_t const sample_rate, time::range const &space_range, float const scale);
    module_location(yas::identifier const &, time::range const &, uint32_t const sample_rate,
                    std::vector<std::optional<mesh_element>> const &mesh_elements, float const scale);

    identifier identifier;  // file_moduleと同じID
    time::range range;      // track内の位置
    uint32_t sample_rate;
    std::vector<std::optional<mesh_element>> mesh_elements;
    float scale;

    float x() const;
    float width() const;
    uint32_t total_rect_count() const;

    std::optional<float> element_offset_x(std::size_t const) const;

    bool operator==(module_location const &rhs) const;
    bool operator!=(module_location const &rhs) const;
};
}  // namespace yas::ae
