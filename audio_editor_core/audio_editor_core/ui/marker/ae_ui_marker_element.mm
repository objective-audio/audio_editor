//
//  ae_ui_marker_element.cpp
//

#include "ae_ui_marker_element.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_marker_element> ui_marker_element::make_shared(args const &args,
                                                                  std::shared_ptr<ui::standard> const &standard,
                                                                  std::shared_ptr<ui::font_atlas> const &font_atlas) {
    auto const &app_lifetime = hierarchy::app_lifetime();

    return std::shared_ptr<ui_marker_element>(
        new ui_marker_element{standard, font_atlas, app_lifetime->color, std::move(args)});
}

ui_marker_element::ui_marker_element(std::shared_ptr<ui::standard> const &standard,
                                     std::shared_ptr<ui::font_atlas> const &font_atlas,
                                     std::shared_ptr<ae::color> const &color, args const &args)
    : node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _triangle_node(ui::node::make_shared()),
      _color(color) {
    auto const line_mesh =
        ui::mesh::make_shared({.primitive_type = args.vertical_line_data->primitive_type},
                              args.vertical_line_data->vertex_data, args.vertical_line_data->index_data, nullptr);
    this->_line_node->set_mesh(line_mesh);

    auto const triangle_mesh =
        ui::mesh::make_shared({.primitive_type = args.triangle_data->primitive_type}, args.triangle_data->vertex_data,
                              args.triangle_data->index_data, nullptr);
    this->_triangle_node->set_mesh(triangle_mesh);

    this->node->add_sub_node(this->_line_node);
    this->node->add_sub_node(this->_triangle_node);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            ui::size const scale{.width = 1.0f, .height = region.size.height};
            this->_line_node->set_scale(scale);
        })
        .sync()
        ->add_to(this->_pool);

    this->_triangle_node->attach_y_layout_guide(*standard->view_look()->view_layout_guide()->top());

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            this->_line_node->set_color(this->_color->marker_line());
            this->_triangle_node->set_color(this->_color->marker_triangle());
        })
        .sync()
        ->add_to(this->_pool);
}
