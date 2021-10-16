//
//  ae_ui_markers.mm
//

#include "ae_ui_markers.h"
#include <audio_editor_core/ae_markers_presenter.h>

using namespace yas;
using namespace yas::ae;

ui_markers::ui_markers(std::size_t const max_count, std::shared_ptr<markers_presenter> const &presenter)
    : _presenter(presenter),
      _max_count(max_count),
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

    this->_presenter->observe_markers([this](auto const &elements) { this->set_elements(elements); })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui_markers> ui_markers::make_shared(std::string const &project_id) {
    auto const presenter = markers_presenter::make_shared(project_id);
    return std::shared_ptr<ui_markers>(new ui_markers{10, presenter});
}

std::shared_ptr<ui::node> const &ui_markers::node() const {
    return this->_root_node;
}

void ui_markers::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_update_sub_nodes();
}

void ui_markers::set_elements(std::vector<marker_location> const &elements) {
    this->_elements = elements;
    this->_update_sub_nodes();
}

void ui_markers::_update_sub_nodes() {
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
        node->set_position({element.x * this->_scale.width, this->_scale.height});
        this->_root_node->add_sub_node(node);
        this->_sub_nodes.emplace_back(std::move(node));
    }
}
