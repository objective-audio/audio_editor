//
//  ae_ui_edge.mm
//

#include "ae_ui_edge.h"
#include <audio_editor_core/ae_edge_presenter.h>
#include <audio_editor_core/ae_ui_edge_element.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_edge> ui_edge::make_shared(ui_project_id const &project_id) {
    auto const &ui_root_level = ui_hierarchy::root_level_for_view_id(project_id.view_id);
    auto const &vertical_line_data = ui_root_level->vertical_line_data;

    ui_edge_element::args const args{.vertical_line_data = vertical_line_data};

    auto const begin_edge = ui_edge_element::make_shared("BEGIN", args, project_id.view_id);
    auto const end_edge = ui_edge_element::make_shared("END", args, project_id.view_id);

    auto const presenter = edge_presenter::make_shared(project_id.project_id, ui_root_level->display_space);
    return std::shared_ptr<ui_edge>(new ui_edge{presenter, begin_edge, end_edge});
}

ui_edge::ui_edge(std::shared_ptr<edge_presenter> const &presenter, std::shared_ptr<ui_edge_element> const &begin_edge,
                 std::shared_ptr<ui_edge_element> const &end_edge)
    : node(ui::node::make_shared()), _presenter(presenter), _begin_edge(begin_edge), _end_edge(end_edge) {
    this->node->add_sub_node(this->_begin_edge->node);
    this->node->add_sub_node(this->_end_edge->node);

    this->_presenter
        ->observe_locations([this](edge_locations const &locations) {
            this->_begin_edge->node->set_position({.x = locations.begin.x, .y = 0.0f});
            this->_end_edge->node->set_position({.x = locations.end.x, .y = 0.0f});
        })
        .sync()
        ->add_to(this->_pool);
}
