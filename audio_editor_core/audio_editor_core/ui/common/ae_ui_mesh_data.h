//
//  ae_ui_mesh_data.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_mesh_data final {
    ui::primitive_type const primitive_type;
    std::shared_ptr<ui::mesh_vertex_data> const vertex_data;
    std::shared_ptr<ui::mesh_index_data> const index_data;

    [[nodiscard]] static std::shared_ptr<ui_mesh_data> make_shared(ui::primitive_type const,
                                                                   std::shared_ptr<ui::mesh_vertex_data> const &,
                                                                   std::shared_ptr<ui::mesh_index_data> const &);

   private:
    ui_mesh_data(ui::primitive_type const, std::shared_ptr<ui::mesh_vertex_data> const &,
                 std::shared_ptr<ui::mesh_index_data> const &);
};
}  // namespace yas::ae
