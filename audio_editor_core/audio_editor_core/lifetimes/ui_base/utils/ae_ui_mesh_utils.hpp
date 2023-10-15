//
//  ae_ui_mesh_utils.hpp
//

#pragma once

#include <audio_editor_core/ae_ui_types.h>

#include <audio_editor_core/ae_ui_dynamic_mesh_container.hpp>

namespace yas::ae::ui_mesh_utils {
std::unique_ptr<dynamic_mesh_container<vertex2d_rect, fill_index2d_rect>> make_fill_container(
    std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>> *vertex_datas, std::size_t const interval,
    bool const use_mesh_color);
std::unique_ptr<dynamic_mesh_container<vertex2d_rect, frame_index2d_rect>> make_frame_container(
    std::vector<std::shared_ptr<ui::dynamic_mesh_vertex_data>> *vertex_datas, std::size_t const interval);
}  // namespace yas::ae::ui_mesh_utils
