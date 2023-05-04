//
//  ae_ui_playing_line.cpp
//

#include "ae_ui_playing_line.hpp"
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_playing_line_utils.h>
#include <audio_editor_core/ae_playing_line_presenter.hpp>
#include <audio_editor_core/ae_ui_square_mesh_data.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_playing_line> ui_playing_line::make_shared(project_lifetime_id const &lifetime_id,
                                                              std::shared_ptr<ui::node> const &node) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_project_lifetime_id(lifetime_id);
    auto const presenter = playing_line_presenter::make_shared(lifetime_id);

    return std::make_shared<ui_playing_line>(app_lifetime->color.get(), presenter, resource_lifetime->standard, node,
                                             resource_lifetime->square_mesh_data);
}

ui_playing_line::ui_playing_line(ae::color *color, std::shared_ptr<playing_line_presenter> const &presenter,
                                 std::shared_ptr<ui::standard> const &standard, std::shared_ptr<ui::node> const &node,
                                 std::shared_ptr<ui_square_mesh_data> const &square_mesh_data)
    : _color(color), _presenter(presenter), _node(ui::node::make_shared()) {
    auto const mesh =
        ui::mesh::make_shared({.primitive_type = ui::primitive_type::triangle}, square_mesh_data->vertex_data(),
                              square_mesh_data->index_data(), square_mesh_data->texture());
    this->_node->set_meshes({mesh});

    node->add_sub_node(this->_node);

    standard->renderer()
        ->observe_will_render([this](auto const &) {
            this->_node->set_color(
                ui_playing_line_utils::to_playing_line_color(this->_presenter->playing_line_state(), this->_color));
        })
        .end()
        ->add_to(this->_pool);

    standard->view_look()
        ->safe_area_layout_guide()
        ->observe([this](ui::region const &src_region) {
            this->_node->set_position({.x = src_region.center().x - 0.5f, .y = src_region.origin.y});
            this->_node->set_scale({.width = 1.0f, .height = src_region.size.height});
        })
        .sync()
        ->add_to(this->_pool);
}
