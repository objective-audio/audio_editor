//
//  ae_waveform_mesh_importer_types.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct waveform_mesh_importer_event {
    static uint32_t const data_unit_max_rect_count = 32;

    std::size_t index;
    identifier identifier;

    struct data {
        std::shared_ptr<ui::mesh_vertex_data> vertex_data;
        std::shared_ptr<ui::mesh_index_data> index_data;
    };

    std::vector<data> datas;
};
}  // namespace yas::ae
