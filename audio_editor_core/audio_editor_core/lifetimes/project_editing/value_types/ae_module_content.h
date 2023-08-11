//
//  ae_module_content.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>
#include <ui/yas_ui_types.h>

#include <audio_editor_core/ae_module_index.hpp>

namespace yas::ae {
class module_object;

struct module_content final {
    /// content内に波形を表示するmeshスライスの位置情報
    struct mesh_element final {
        static uint32_t const max_length = 512;

        uint32_t rect_count;
        time::range range;  // module_content内の位置

        bool operator==(mesh_element const &rhs) const;
        bool operator!=(mesh_element const &rhs) const;
    };

    module_content(module_object const &, bool const is_selected, uint32_t const sample_rate,
                   time::range const &space_range, ui::size const scale);
    module_content(object_id const &, time::range const &, track_index_t const track, bool const is_selected,
                   uint32_t const sample_rate, std::vector<std::optional<mesh_element>> const &mesh_elements,
                   ui::size const scale);

    object_id identifier;  // moduleと同じID
    time::range range;     // track内の位置
    track_index_t track;
    bool is_selected;
    uint32_t sample_rate;
    std::vector<std::optional<mesh_element>> mesh_elements;
    ui::size scale;

    [[nodiscard]] module_index index() const;

    [[nodiscard]] ui::region region() const;
    [[nodiscard]] uint32_t total_rect_count() const;

    [[nodiscard]] std::optional<float> element_offset_x(std::size_t const) const;

    bool operator==(module_content const &rhs) const;
    bool operator!=(module_content const &rhs) const;
};
}  // namespace yas::ae
