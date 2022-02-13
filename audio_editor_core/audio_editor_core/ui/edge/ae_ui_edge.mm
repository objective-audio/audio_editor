//
//  ae_ui_edge.mm
//

#include "ae_ui_edge.h"
#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_edge_presenter.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_edge> ui_edge::make_shared(std::string const &project_id,
                                              std::shared_ptr<ui::standard> const &standard,
                                              std::shared_ptr<display_space> const &display_space) {
    auto const presenter = edge_presenter::make_shared(project_id, display_space);
    auto const color = ae::app::global()->color();
    return std::shared_ptr<ui_edge>(new ui_edge{presenter, standard, color});
}

ui_edge::ui_edge(std::shared_ptr<edge_presenter> const &presenter, std::shared_ptr<ui::standard> const &standard,
                 std::shared_ptr<ae::color> const &color)
    : _presenter(presenter),
      _color(color),
      _node(ui::node::make_shared()),
      _vertex_data(ui::static_mesh_vertex_data::make_shared(2)),
      _index_data(ui::static_mesh_index_data::make_shared(2)),
      _begin_mesh(ui::mesh::make_shared()),
      _begin_node(ui::node::make_shared()),
      _end_mesh(ui::mesh::make_shared()),
      _end_node(ui::node::make_shared()) {
    this->_setup_nodes();
    this->_setup_observing(standard);
}

std::shared_ptr<ui::node> const &ui_edge::node() const {
    return this->_node;
}

void ui_edge::_setup_nodes() {
    this->_vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        vertices.at(0).position = {0.0f, -0.5f};
        vertices.at(1).position = {0.0f, 0.5f};
    });

    this->_index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices.at(0) = 0;
        indices.at(1) = 1;
    });

    this->_begin_mesh->set_vertex_data(this->_vertex_data);
    this->_begin_mesh->set_index_data(this->_index_data);
    this->_begin_mesh->set_primitive_type(ui::primitive_type::line);
    this->_begin_node->set_mesh(this->_begin_mesh);

    this->_end_mesh->set_vertex_data(this->_vertex_data);
    this->_end_mesh->set_index_data(this->_index_data);
    this->_end_mesh->set_primitive_type(ui::primitive_type::line);
    this->_end_node->set_mesh(this->_end_mesh);

    this->_node->add_sub_node(this->_begin_node);
    this->_node->add_sub_node(this->_end_node);
}

void ui_edge::_setup_observing(std::shared_ptr<ui::standard> const &standard) {
    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            ui::size const scale{.width = 1.0f, .height = region.size.height};
            this->_begin_node->set_scale(scale);
            this->_end_node->set_scale(scale);
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            auto const color = this->_color->edge_line();
            this->_begin_node->set_color(color);
            this->_end_node->set_color(color);
        })
        .sync()
        ->add_to(this->_pool);

    this->_presenter
        ->observe_locations([this](edge_locations const &locations) {
            this->_begin_node->set_position({.x = locations.begin.x, .y = 0.0f});
            this->_end_node->set_position({.x = locations.end.x, .y = 0.0f});
        })
        .sync()
        ->add_to(this->_pool);
}
