//
//  ae_ui_pasting_marker_element.cpp
//

#include "ae_ui_pasting_marker_element.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <cpp_utils/yas_assertion.h>
#include <audio_editor_core/ae_ui_square_mesh_data.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_pasting_marker_element> ui_pasting_marker_element::make_shared(
    project_lifetime_id const &lifetime_id, ui::node *parent_node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);

    return std::make_shared<ui_pasting_marker_element>(resource_lifetime->standard, parent_node,
                                                       resource_lifetime->square_mesh_data);
}

ui_pasting_marker_element::ui_pasting_marker_element(std::shared_ptr<ui::standard> const &standard,
                                                     ui::node *parent_node,
                                                     std::shared_ptr<ui_square_mesh_data> const &square_mesh_data)
    : _node(ui::node::make_shared()), _line_node(ui::node::make_shared()) {
    parent_node->add_sub_node(this->_node);

    auto const line_mesh =
        ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle}, square_mesh_data->vertex_data(),
                              square_mesh_data->index_data(), square_mesh_data->texture());
    this->_line_node->set_meshes({line_mesh});

    this->_node->add_sub_node(this->_line_node);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            this->_line_node->set_position({.x = 0.0f, .y = -region.size.height * 0.5f});
            this->_line_node->set_scale({.width = 0.5f, .height = region.size.height});
        })
        .sync()
        ->add_to(this->_pool);

    standard->view_look()
        ->observe_appearance([this](auto const &) { this->_update_color(); })
        .sync()
        ->add_to(this->_pool);
}

void ui_pasting_marker_element::set_content(pasting_marker_content const &content) {
    this->_content = content;
    this->_node->set_is_enabled(true);
    this->_node->set_x(content.x);
}

void ui_pasting_marker_element::update_content(pasting_marker_content const &content) {
    if (this->_content.has_value() && this->_content.value().identifier == content.identifier) {
        this->_content = content;
        this->_node->set_x(content.x);
    } else {
        assertion_failure_if_not_test();
    }
}

void ui_pasting_marker_element::reset_content() {
    this->_content = std::nullopt;
    this->_node->set_is_enabled(false);
}

void ui_pasting_marker_element::finalize() {
    this->_node->remove_from_super_node();
}

void ui_pasting_marker_element::_update_color() {
    this->_line_node->set_color(this->_color->pasting_marker_line());
}
