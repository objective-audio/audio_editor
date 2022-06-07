//
//  ae_ui_edge_element.mm
//

#include "ae_ui_edge_element.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <audio_editor_core/ae_ui_root_lifetime.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_edge_element> ui_edge_element::make_shared(std::string const &text, args const &args,
                                                              std::shared_ptr<ui::standard> const &standard,
                                                              std::shared_ptr<ui::font_atlas> const &font_atlas) {
    auto const &app_lifetime = hierarchy::app_lifetime();

    return std::make_shared<ui_edge_element>(text, standard, font_atlas, app_lifetime->color, args);
}

ui_edge_element::ui_edge_element(std::string const &text, std::shared_ptr<ui::standard> const &standard,
                                 std::shared_ptr<ui::font_atlas> const &font_atlas,
                                 std::shared_ptr<ae::color> const &color, args const &args)
    : node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _text(ui::strings::make_shared({.text = text}, font_atlas)),
      _color(color) {
    auto const mesh =
        ui::mesh::make_shared({.primitive_type = args.vertical_line_data->primitive_type},
                              args.vertical_line_data->vertex_data, args.vertical_line_data->index_data, nullptr);
    this->_line_node->set_mesh(mesh);

    this->_text->rect_plane()->node()->set_position({2.0f, 0.0f});

    this->node->add_sub_node(this->_line_node);
    this->node->add_sub_node(this->_text->rect_plane()->node());

    this->_text->rect_plane()->node()->attach_y_layout_guide(*standard->view_look()->view_layout_guide()->top());

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            ui::size const scale{.width = 1.0f, .height = region.size.height};
            this->_line_node->set_scale(scale);
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) {
            this->_line_node->set_color(this->_color->edge_line());
            this->_text->rect_plane()->node()->set_color(this->_color->edge_text());
        })
        .sync()
        ->add_to(this->_pool);
}
