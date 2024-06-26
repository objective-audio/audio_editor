//
//  ui_square_mesh_data.cpp
//

#include "ui_square_mesh_data.hpp"
#include <ae-core/ui_base/features/common/ui_atlas.hpp>

using namespace yas;
using namespace yas::ae;

ui_square_mesh_data::ui_square_mesh_data(ui_atlas const *atlas)
    : _data(ui::rect_plane_data::make_shared(1)), _atlas(atlas) {
    this->_data->set_rect_position(ui::region{.origin = {0.0f, 0.0f}, .size = {1.0f, 1.0f}}, 0);

    atlas
        ->observe_white_filled_tex_coords(
            [this](ui::uint_region const &tex_coords) { this->_data->set_rect_tex_coords(tex_coords, 0); })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::mesh_vertex_data> ui_square_mesh_data::vertex_data() const {
    return this->_data->dynamic_vertex_data();
}

std::shared_ptr<ui::mesh_index_data> ui_square_mesh_data::index_data() const {
    return this->_data->dynamic_index_data();
}

std::shared_ptr<ui::texture> const &ui_square_mesh_data::texture() const {
    return this->_atlas->texture();
}
