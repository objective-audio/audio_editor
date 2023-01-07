//
//  ae_ui_edge.mm
//

#include "ae_ui_edge.h"
#include <audio_editor_core/ae_edge_presenter.h>
#include <audio_editor_core/ae_ui_edge_element.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_mesh_data.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_edge> ui_edge::make_shared(window_lifetime_id const &window_lifetime_id,
                                              std::shared_ptr<ui::node> const &node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(window_lifetime_id);

    ui_edge_element::args const args{.square_mesh_data = resource_lifetime->square_mesh_data};

    auto const begin_edge =
        ui_edge_element::make_shared("BEGIN", args, resource_lifetime->standard, resource_lifetime->normal_font_atlas);
    auto const end_edge =
        ui_edge_element::make_shared("END", args, resource_lifetime->standard, resource_lifetime->normal_font_atlas);

    auto const presenter = edge_presenter::make_shared(window_lifetime_id, resource_lifetime->display_space);
    return std::make_shared<ui_edge>(presenter, begin_edge, end_edge, node);
}

ui_edge::ui_edge(std::shared_ptr<edge_presenter> const &presenter, std::shared_ptr<ui_edge_element> const &begin_edge,
                 std::shared_ptr<ui_edge_element> const &end_edge, std::shared_ptr<ui::node> const &node)
    : _presenter(presenter), _begin_edge(begin_edge), _end_edge(end_edge) {
    node->add_sub_node(this->_begin_edge->node);
    node->add_sub_node(this->_end_edge->node);

    this->_presenter
        ->observe_locations([this](edge_locations const &locations) {
            this->_begin_edge->node->set_position({.x = locations.begin.x, .y = 0.0f});
            this->_end_edge->node->set_position({.x = locations.end.x, .y = 0.0f});
        })
        .sync()
        ->add_to(this->_pool);
}
