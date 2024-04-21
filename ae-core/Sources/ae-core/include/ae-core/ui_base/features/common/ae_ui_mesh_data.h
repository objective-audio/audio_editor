//
//  ae_ui_mesh_data.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_mesh_data final {
    ui_mesh_data(ui::primitive_type const, std::shared_ptr<ui::mesh_vertex_data> const &,
                 std::shared_ptr<ui::mesh_index_data> const &);

    ui::primitive_type const primitive_type;
    std::shared_ptr<ui::mesh_vertex_data> const vertex_data;
    std::shared_ptr<ui::mesh_index_data> const index_data;

   private:
    ui_mesh_data(ui_mesh_data const &) = delete;
    ui_mesh_data(ui_mesh_data &&) = delete;
    ui_mesh_data &operator=(ui_mesh_data const &) = delete;
    ui_mesh_data &operator=(ui_mesh_data &&) = delete;
};
}  // namespace yas::ae
