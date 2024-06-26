//
//  ui_node_hierarchy.cpp
//

#include "ui_node_hierarchy.hpp"
#include <ae-core/global/ui_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_node_hierarchy> ui_node_hierarchy::make_shared(project_lifetime_id const &lifetime_id) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);
    return std::make_shared<ui_node_hierarchy>(resource_lifetime->standard->root_node());
}

ui_node_hierarchy::ui_node_hierarchy(std::shared_ptr<ui::node> const &root_node)
    : base_node(ui::node::make_shared()),
      range_selection_input_node(ui::node::make_shared()),
      scroller_nodes({.back_x_fixed = ui::node::make_shared(),
                      .back_y_fixed = ui::node::make_shared(),
                      .modules = ui::node::make_shared(),
                      .front_y_fixed = ui::node::make_shared(),
                      .front_x_fixed = ui::node::make_shared()}),
      pasting_modules_node(ui::node::make_shared()),
      pasting_markers_node(ui::node::make_shared()),
      playing_line_node(ui::node::make_shared()),
      modal_bg_node(ui::node::make_shared()),
      time_base_node(ui::node::make_shared()),
      tracks_bg_node(ui::node::make_shared()),
      grid_node(ui::node::make_shared()),
      modules_node(ui::node::make_shared()),
      range_selection_node(ui::node::make_shared()),
      edge_node(ui::node::make_shared()),
      markers_node(ui::node::make_shared()),
      tracks_front_node(ui::node::make_shared()),
      time_bg_node(ui::node::make_shared()),
      time_buttons_node(ui::node::make_shared()),
      time_nudge_node(ui::node::make_shared()),
      time_strings_node(ui::node::make_shared()) {
    root_node->add_sub_node(this->base_node);

    this->base_node->add_sub_node(this->range_selection_input_node);
    this->base_node->add_sub_node(this->scroller_nodes.back_x_fixed);
    this->base_node->add_sub_node(this->scroller_nodes.back_y_fixed);
    this->base_node->add_sub_node(this->scroller_nodes.modules);
    this->base_node->add_sub_node(this->scroller_nodes.front_y_fixed);
    this->base_node->add_sub_node(this->scroller_nodes.front_x_fixed);
    this->base_node->add_sub_node(this->pasting_modules_node);
    this->base_node->add_sub_node(this->pasting_markers_node);
    this->base_node->add_sub_node(this->playing_line_node);
    this->base_node->add_sub_node(this->modal_bg_node);
    this->base_node->add_sub_node(this->time_base_node);

    this->scroller_nodes.back_x_fixed->add_sub_node(this->tracks_bg_node);
    this->scroller_nodes.back_y_fixed->add_sub_node(this->grid_node);
    this->scroller_nodes.modules->add_sub_node(this->modules_node);
    this->scroller_nodes.modules->add_sub_node(this->range_selection_node);
    this->scroller_nodes.front_y_fixed->add_sub_node(this->edge_node);
    this->scroller_nodes.front_y_fixed->add_sub_node(this->markers_node);
    this->scroller_nodes.front_x_fixed->add_sub_node(this->tracks_front_node);

    this->time_base_node->add_sub_node(this->time_bg_node);
    this->time_base_node->add_sub_node(this->time_buttons_node);
    this->time_base_node->add_sub_node(this->time_nudge_node);
    this->time_base_node->add_sub_node(this->time_strings_node);

    this->modules_node->set_batch(ui::batch::make_shared());
    this->pasting_modules_node->set_batch(ui::batch::make_shared());
    this->markers_node->set_batch(ui::batch::make_shared());
    this->pasting_markers_node->set_batch(ui::batch::make_shared());
    this->time_base_node->set_batch(ui::batch::make_shared());
    this->tracks_front_node->set_batch(ui::batch::make_shared());
}
