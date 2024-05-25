//
//  ui_framed_square.mm
//

#include "ui_framed_square.hpp"
#include <ae-core/ui_base/value_types/ui_types.h>
#include <ae-core/ui_resource/features/ui_square_mesh_data.hpp>

using namespace yas;
using namespace yas::ae;

ui_framed_square::ui_framed_square(ui_square_mesh_data const *square_mesh_data)
    : node(ui::node::make_shared()),
      _origin_node(ui::node::make_shared()),
      _fill_node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()) {
    this->node->add_sub_node(this->_origin_node);
    this->_origin_node->add_sub_node(this->_fill_node);
    this->_origin_node->add_sub_node(this->_line_node);

    auto const fill_mesh =
        ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle}, square_mesh_data->vertex_data(),
                              square_mesh_data->index_data(), square_mesh_data->texture());
    this->_fill_node->set_meshes({fill_mesh});

    auto const line_index_data = ui::static_mesh_index_data::make_shared(frame_index2d_rect::vector_count);

    line_index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        auto *index_rects = (frame_index2d_rect *)indices.data();
        index_rects[0].set_all(0);
    });

    auto const line_mesh =
        ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, square_mesh_data->vertex_data(),
                              line_index_data, square_mesh_data->texture());
    this->_line_node->set_meshes({line_mesh});
}

void ui_framed_square::set_region(ui::region const &region) {
    this->_origin_node->set_position(region.origin);
    this->_fill_node->set_scale(region.size);
    this->_line_node->set_scale(region.size);
}

void ui_framed_square::set_fill_color(ui::color const &color) {
    this->_fill_node->set_color(color);
}

void ui_framed_square::set_frame_color(ui::color const &color) {
    this->_line_node->set_color(color);
}
