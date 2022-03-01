//
//  ae_ui_edge_element.mm
//

#include "ae_ui_edge_element.h"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_root_level.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_edge_element> ui_edge_element::make_shared(std::string const &text, args const &args,
                                                              uintptr_t const project_view_id) {
    auto const &app_level = app_level::global();
    auto const &ui_root_level = hierarchy::ui_root_level_for_view_id(project_view_id);

    return std::shared_ptr<ui_edge_element>(
        new ui_edge_element{text, ui_root_level->standard, ui_root_level->font_atlas_14, app_level->color, args});
}

ui_edge_element::ui_edge_element(std::string const &text, std::shared_ptr<ui::standard> const &standard,
                                 std::shared_ptr<ui::font_atlas> const &font_atlas,
                                 std::shared_ptr<ae::color> const &color, args const &args)
    : _mesh(ui::mesh::make_shared()),
      _node(ui::node::make_shared()),
      _line_node(ui::node::make_shared()),
      _text(ui::strings::make_shared({.text = text}, font_atlas)),
      _color(color) {
    this->_mesh->set_vertex_data(args.line_vertex_data);
    this->_mesh->set_index_data(args.line_index_data);
    this->_mesh->set_primitive_type(ui::primitive_type::line);
    this->_line_node->set_mesh(this->_mesh);

    this->_text->rect_plane()->node()->set_position({2.0f, 0.0f});

    this->_node->add_sub_node(this->_line_node);
    this->_node->add_sub_node(this->_text->rect_plane()->node());

    this->_text->rect_plane()->node()->attach_y_layout_guide(*args.top_guide);

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

std::shared_ptr<ui::node> const &ui_edge_element::node() const {
    return this->_node;
}
