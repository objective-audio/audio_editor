//
//  ui_square_mesh_data.hpp
//

#pragma once

#include <ui/yas_ui_umbrella.h>

#include <observing/umbrella.hpp>

namespace yas::ae {
class ui_atlas;

struct ui_square_mesh_data final {
    ui_square_mesh_data(ui_atlas const *);

    [[nodiscard]] std::shared_ptr<ui::mesh_vertex_data> vertex_data() const;
    [[nodiscard]] std::shared_ptr<ui::mesh_index_data> index_data() const;

    [[nodiscard]] std::shared_ptr<ui::texture> const &texture() const;

   private:
    ui_square_mesh_data(ui_square_mesh_data const &) = delete;
    ui_square_mesh_data(ui_square_mesh_data &&) = delete;
    ui_square_mesh_data &operator=(ui_square_mesh_data const &) = delete;
    ui_square_mesh_data &operator=(ui_square_mesh_data &&) = delete;

    std::shared_ptr<ui::rect_plane_data> const _data;
    ui_atlas const *const _atlas;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
