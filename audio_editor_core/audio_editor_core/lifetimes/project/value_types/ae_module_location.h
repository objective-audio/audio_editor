//
//  ae_module_location.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>

#include <audio_editor_core/ae_file_module_index.hpp>

namespace yas::ae {
class file_module_object;

struct module_location final {
    struct mesh_element final {
        static uint32_t const max_length = 512;

        uint32_t rect_count;
        time::range range;  // module_location内の位置

        bool operator==(mesh_element const &rhs) const;
        bool operator!=(mesh_element const &rhs) const;
    };

    module_location(file_module_object const &, uint32_t const sample_rate, time::range const &space_range,
                    float const scale);
    module_location(object_id const &, time::range const &, uint32_t const sample_rate,
                    std::vector<std::optional<mesh_element>> const &mesh_elements, float const scale);

    object_id identifier;  // file_moduleと同じID
    time::range range;     // track内の位置
    uint32_t sample_rate;
    std::vector<std::optional<mesh_element>> mesh_elements;
    float scale;

    [[nodiscard]] file_module_index index() const;

    [[nodiscard]] float x() const;
    [[nodiscard]] float width() const;
    [[nodiscard]] uint32_t total_rect_count() const;

    [[nodiscard]] std::optional<float> element_offset_x(std::size_t const) const;

    bool operator==(module_location const &rhs) const;
    bool operator!=(module_location const &rhs) const;
};
}  // namespace yas::ae
