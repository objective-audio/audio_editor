//
//  ae_ui_marker_plane.mm
//

#include "ae_ui_marker_plane.h"

using namespace yas;
using namespace yas::ae;

ui_marker_plane::ui_marker_plane(std::size_t const max_count)
    : _max_count(max_count),
      _root_node(ui::node::make_shared()),
      _vertex_data(ui::static_mesh_vertex_data::make_shared(3)),
      _index_data(ui::static_mesh_index_data::make_shared(3)) {
    this->_vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        float const half_width = -5.0f;
        float const height = 10.0f;
        vertices[0].position = {0.0f, 0.0f};
        vertices[1].position = {-half_width, height};
        vertices[2].position = {half_width, height};
    });

    this->_index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
    });
}

std::shared_ptr<ui_marker_plane> ui_marker_plane::make_shared(std::size_t const max_count) {
    return std::shared_ptr<ui_marker_plane>(new ui_marker_plane{max_count});
}

std::shared_ptr<ui::node> const &ui_marker_plane::node() const {
    return this->_root_node;
}

void ui_marker_plane::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_update_sub_nodes();
}

void ui_marker_plane::set_elements(std::vector<marker_element> &&elements) {
    this->_elements = std::move(elements);
    this->_update_sub_nodes();
}

void ui_marker_plane::_update_sub_nodes() {
    for (auto const &node : this->_sub_nodes) {
        node->remove_from_super_node();
    }
    this->_sub_nodes.clear();

    auto const count = std::min(this->_elements.size(), this->_max_count);
    auto each = make_fast_each(count);
    while (yas_each_next(each)) {
        auto const &element = this->_elements.at(yas_each_index(each));
        auto node = ui::node::make_shared();
        node->set_mesh(ui::mesh::make_shared({}, this->_vertex_data, this->_index_data, nullptr));
        node->set_position({element.position * this->_scale.width, this->_scale.height});
        this->_root_node->add_sub_node(node);
        this->_sub_nodes.emplace_back(std::move(node));
    }
}
