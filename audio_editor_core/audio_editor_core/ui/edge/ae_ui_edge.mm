//
//  ae_ui_edge.mm
//

#include "ae_ui_edge.h"
#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_edge_presenter.h>
#include <audio_editor_core/ae_ui_edge_element.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_pool.h>
#include <audio_editor_core/ae_ui_root.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_edge> ui_edge::make_shared(std::string const &project_id, uintptr_t const project_view_id) {
    auto const &app = app::global();
    auto const &ui_root = app->ui_pool()->ui_root_for_view_id(project_view_id);
    auto const &standard = ui_root->standard();
    auto const &display_space = ui_root->display_space();
    auto const color = app->color();

    auto const top_guide = ui::layout_value_guide::make_shared();

    auto const vertex_data = ui::static_mesh_vertex_data::make_shared(2);
    auto const index_data = ui::static_mesh_index_data::make_shared(2);

    vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        vertices.at(0).position = {0.0f, -0.5f};
        vertices.at(1).position = {0.0f, 0.5f};
    });

    index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices.at(0) = 0;
        indices.at(1) = 1;
    });

    ui_edge_element::args const args{
        .line_vertex_data = vertex_data, .line_index_data = index_data, .top_guide = top_guide};

    auto const begin_edge = ui_edge_element::make_shared("BEGIN", args, project_view_id);
    auto const end_edge = ui_edge_element::make_shared("END", args, project_view_id);

    auto const presenter = edge_presenter::make_shared(project_id, display_space);
    return std::shared_ptr<ui_edge>(new ui_edge{presenter, standard, top_guide, begin_edge, end_edge});
}

ui_edge::ui_edge(std::shared_ptr<edge_presenter> const &presenter, std::shared_ptr<ui::standard> const &standard,
                 std::shared_ptr<ui::layout_value_guide> const &top_guide,
                 std::shared_ptr<ui_edge_element> const &begin_edge, std::shared_ptr<ui_edge_element> const &end_edge)
    : _presenter(presenter),
      _top_guide(top_guide),
      _node(ui::node::make_shared()),
      _begin_edge(begin_edge),
      _end_edge(end_edge) {
    this->_node->add_sub_node(this->_begin_edge->node());
    this->_node->add_sub_node(this->_end_edge->node());

    ui::layout(standard->view_look()->view_layout_guide()->top(), this->_top_guide, ui_layout_utils::constant(0.0f))
        .sync()
        ->add_to(this->_pool);

    this->_presenter
        ->observe_locations([this](edge_locations const &locations) {
            this->_begin_edge->node()->set_position({.x = locations.begin.x, .y = 0.0f});
            this->_end_edge->node()->set_position({.x = locations.end.x, .y = 0.0f});
        })
        .sync()
        ->add_to(this->_pool);
}

std::shared_ptr<ui::node> const &ui_edge::node() const {
    return this->_node;
}
