//
//  ae_module_location.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <audio_editor_core/ae_module_location.h>
#include <processing/yas_processing_umbrella.h>

namespace yas::ae {
class file_module;

struct module_location {
    struct mesh_element {
        static uint32_t const max_length = 512;

        uint32_t rect_count;
        proc::time::range range;  // module_location内の位置

        bool operator==(mesh_element const &rhs) const;
        bool operator!=(mesh_element const &rhs) const;
    };

    identifier identifier;    // file_moduleと同じID
    proc::time::range range;  // track内の位置
    proc::frame_index_t file_frame;
    uint32_t sample_rate;
    std::vector<std::optional<mesh_element>> mesh_elements;
    float scale;

    [[nodiscard]] static module_location make_value(ae::identifier const &, proc::time::range const &,
                                                    proc::frame_index_t const file_frame, uint32_t const sample_rate,
                                                    std::vector<std::optional<mesh_element>> const &mesh_elements,
                                                    float const scale);
    [[nodiscard]] static module_location make_value(file_module const &, uint32_t const sample_rate,
                                                    proc::time::range const &space_range, float const scale);

    float x() const;
    float width() const;
    uint32_t total_rect_count() const;

    std::optional<float> element_offset_x(std::size_t const) const;

    bool operator==(module_location const &rhs) const;
    bool operator!=(module_location const &rhs) const;
};
}  // namespace yas::ae
