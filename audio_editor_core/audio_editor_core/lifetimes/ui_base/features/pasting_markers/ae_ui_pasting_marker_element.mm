//
//  ae_ui_pasting_marker_element.cpp
//

#include "ae_ui_pasting_marker_element.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_pasting_marker_element> ui_pasting_marker_element::make_shared(window_lifetime_id const &lifetime_id,
                                                                                  ui::node *parent_node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);

    return std::make_shared<ui_pasting_marker_element>(resource_lifetime->standard, parent_node,
                                                       resource_lifetime->vertical_line_data);
}

ui_pasting_marker_element::ui_pasting_marker_element(std::shared_ptr<ui::standard> const &standard,
                                                     ui::node *parent_node,
                                                     std::shared_ptr<ui_mesh_data> const &vertical_line_data)
    : _node(ui::node::make_shared()), _line_node(ui::node::make_shared()) {
    parent_node->add_sub_node(this->_node);

    auto const line_mesh =
        ui::mesh::make_shared({.primitive_type = vertical_line_data->primitive_type}, vertical_line_data->vertex_data,
                              vertical_line_data->index_data, nullptr);
    this->_line_node->set_mesh(line_mesh);

    this->_node->add_sub_node(this->_line_node);

    standard->view_look()
        ->view_layout_guide()
        ->observe([this](ui::region const &region) {
            ui::size const scale{.width = 1.0f, .height = region.size.height};
            this->_line_node->set_scale(scale);
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
    this->_node->set_position({content.x, this->_node->position().y});
}

void ui_pasting_marker_element::update_content(pasting_marker_content const &content) {
    if (this->_content.has_value() && this->_content.value().identifier == content.identifier) {
        this->_content = content;
        this->_node->set_position({content.x, this->_node->position().y});
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
