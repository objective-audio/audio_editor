//
//  waveform_mesh_importer_types.h
//

#pragma once

#include <cpp-utils/identifier.h>
#include <ui/common/yas_ui_types.h>

namespace yas::ae {
struct waveform_mesh_data final {
    std::shared_ptr<ui::dynamic_mesh_vertex_data> vertex_data;
    std::shared_ptr<ui::mesh_index_data> index_data;
};

struct waveform_mesh_importer_event final {
    std::size_t index;
    object_id identifier;

    std::vector<waveform_mesh_data> datas;
};
}  // namespace yas::ae
