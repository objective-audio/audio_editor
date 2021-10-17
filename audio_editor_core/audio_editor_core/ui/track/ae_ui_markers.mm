//
//  ae_ui_markers.mm
//

#include "ae_ui_markers.h"
#include <audio_editor_core/ae_common_utils.h>
#include <audio_editor_core/ae_markers_presenter.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_markers_constants {
static std::size_t const reserving_interval = 10;
}

ui_markers::ui_markers(std::shared_ptr<markers_presenter> const &presenter)
    : _presenter(presenter),
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

    this->_presenter->observe_markers([this](auto const &locations) { this->set_locations(locations); })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui_markers> ui_markers::make_shared(std::string const &project_id) {
    auto const presenter = markers_presenter::make_shared(project_id);
    return std::shared_ptr<ui_markers>(new ui_markers{presenter});
}

std::shared_ptr<ui::node> const &ui_markers::node() const {
    return this->_root_node;
}

void ui_markers::set_scale(ui::size const &scale) {
    this->_scale = scale;
    this->_update_sub_nodes();
}

void ui_markers::set_locations(std::vector<marker_location> const &locations) {
    this->_locations = locations;
    this->_update_sub_nodes();
}

void ui_markers::_update_sub_nodes() {
    auto const node_count = this->_sub_nodes.size();
    auto const location_count = this->_locations.size();

    if (node_count < location_count) {
        this->_sub_nodes.reserve(
            common_utils::reserving_count(location_count, ui_markers_constants::reserving_interval));

        auto each = make_fast_each(location_count - node_count);
        while (yas_each_next(each)) {
            auto node = ui::node::make_shared();
            node->set_mesh(ui::mesh::make_shared({}, this->_vertex_data, this->_index_data, nullptr));
            this->_root_node->add_sub_node(node);
            this->_sub_nodes.emplace_back(std::move(node));
        }
    } else if (location_count < node_count) {
        auto each = make_fast_each(node_count - location_count);
        while (yas_each_next(each)) {
            auto const idx = node_count - 1 - yas_each_index(each);
            this->_sub_nodes.at(idx)->remove_from_super_node();
        }
        this->_sub_nodes.resize(location_count);
    }

    auto each = make_fast_each(location_count);
    while (yas_each_next(each)) {
        auto const &idx = yas_each_index(each);
        auto const &location = this->_locations.at(idx);
        auto const &node = this->_sub_nodes.at(idx);
        node->set_position({location.x * this->_scale.width, this->_scale.height});
    }
}
