//
//  ae_waveform_mesh_importer_types.h
//

#pragma once

#include <audio_editor_core/ae_identifier.h>
#include <ui/yas_ui_types.h>

namespace yas::ae {
struct waveform_mesh_importer_event {
    std::size_t index;
    identifier identifier;
    std::shared_ptr<ui::static_mesh_vertex_data> vertex_data;
    std::shared_ptr<ui::static_mesh_index_data> lines_index_data;
};
}  // namespace yas::ae
