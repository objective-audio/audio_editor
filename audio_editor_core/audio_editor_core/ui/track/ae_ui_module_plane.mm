//
//  ae_ui_module_plane.mm
//

#include "ae_ui_module_plane.h"
#include <audio_editor_core/ae_module_element.h>

using namespace yas;
using namespace yas::ae;

void ui_module_plane::line_index2d_rect::set_all(ui::index2d_t const first) {
    this->v[0] = this->v[7] = first;
    this->v[1] = this->v[2] = first + 1;
    this->v[3] = this->v[4] = first + 3;
    this->v[5] = this->v[6] = first + 2;
}

ui_module_plane::ui_module_plane(std::size_t const max_count)
    : _max_count(max_count),
      _node(ui::node::make_shared()),
      _triangle_node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _vertex_data(ui::dynamic_mesh_vertex_data::make_shared(max_count * 4)),
      _triangle_index_data(ui::dynamic_mesh_index_data::make_shared(max_count * 6)),
      _line_index_data(ui::dynamic_mesh_index_data::make_shared(max_count * 8)),
      _triangle_mesh(
          ui::mesh::make_shared({.use_mesh_color = true}, this->_vertex_data, this->_triangle_index_data, nullptr)),
      _line_mesh(ui::mesh::make_shared({.primitive_type = ui::primitive_type::line}, this->_vertex_data,
                                       this->_line_index_data, nullptr)) {
    this->_node->add_sub_node(this->_triangle_node);
    this->_node->add_sub_node(this->_line_node);

    this->_triangle_node->set_mesh(this->_triangle_mesh);
    this->_line_node->set_mesh(this->_line_mesh);
    this->_line_node->set_color(ui::white_color());

    this->_set_rect_count(0);

    this->_write_vertices([&max_count](vertex2d_rect *vertex_rects) {
        auto rect_each = make_fast_each(max_count);
        while (yas_each_next(rect_each)) {
            auto const &rect_idx = yas_each_index(rect_each);
            auto &vertex_rect = vertex_rects[rect_idx];

            float const hue = float(rect_idx % 6) / 6.0f;
            auto const color = ui::hsb_color(hue, 1.0f, 1.0f);

            auto each = make_fast_each(4);
            while (yas_each_next(each)) {
                vertex_rect.v[yas_each_index(each)].color = to_float4(color, 0.3f);
            }
        }
    });

    this->_write_triangle_indices([&max_count](triangle_index2d_rect *index_rects) {
        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * 4);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });

    this->_write_line_indices([&max_count](line_index2d_rect *index_rects) {
        auto each = make_fast_each(max_count);
        while (yas_each_next(each)) {
            auto const &rect_idx = yas_each_index(each);
            uint32_t const rect_head_idx = static_cast<uint32_t>(rect_idx * 4);
            index_rects[rect_idx].set_all(rect_head_idx);
        }
    });
}

std::shared_ptr<ui_module_plane> ui_module_plane::make_shared(std::size_t const max_count) {
    return std::shared_ptr<ui_module_plane>(new ui_module_plane{max_count});
}

std::shared_ptr<ui::node> const &ui_module_plane::node() const {
    return this->_node;
}

void ui_module_plane::set_scale(ui::size const &scale) {
    this->_node->set_scale(scale);
}

void ui_module_plane::set_elements(std::vector<module_element> const &elements) {
    this->_write_vertices([&elements](vertex2d_rect *vertex_rects) {
        auto each = make_fast_each(elements.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            auto const &element = elements.at(idx);
            vertex_rects[idx].set_position(ui::region{.origin = {.x = element.position, .y = 0.0f},
                                                      .size = {.width = element.length, .height = 1.0f}});
        }
    });

    this->_set_rect_count(elements.size());
}

void ui_module_plane::_set_rect_count(std::size_t const rect_count) {
    this->_vertex_data->set_count(rect_count * 4);
    this->_triangle_index_data->set_count(rect_count * 6);
    this->_line_index_data->set_count(rect_count * 8);
}

void ui_module_plane::_write_vertices(std::function<void(vertex2d_rect *)> const &handler) {
    this->_vertex_data->write([&handler](auto &vertices) { handler((ui::vertex2d_rect *)vertices.data()); });
}

void ui_module_plane::_write_triangle_indices(std::function<void(triangle_index2d_rect *)> const &handler) {
    this->_triangle_index_data->write([&handler](auto &indices) { handler((triangle_index2d_rect *)indices.data()); });
}

void ui_module_plane::_write_line_indices(std::function<void(line_index2d_rect *)> const &handler) {
    this->_line_index_data->write([&handler](auto &indices) { handler((line_index2d_rect *)indices.data()); });
}
