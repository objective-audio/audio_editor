//
//  ae_ui_resource_lifetime.cpp
//

#include "ae_ui_resource_lifetime.hpp"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <audio_editor_core/ae_ui_base_lifecycle.hpp>

using namespace yas;
using namespace yas::ae;

namespace yas::ae::ui_resource_lifetime_utils {
std::shared_ptr<ui_mesh_data> make_vertical_line_data() {
    auto const vertex_data = ui::static_mesh_vertex_data::make_shared(2);
    auto const index_data = ui::static_mesh_index_data::make_shared(2);

    auto mesh_data = ui_mesh_data::make_shared(ui::primitive_type::line, vertex_data, index_data);
    vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        vertices.at(0).position = {0.0f, -0.5f};
        vertices.at(1).position = {0.0f, 0.5f};
    });
    index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices.at(0) = 0;
        indices.at(1) = 1;
    });
    return mesh_data;
}

std::shared_ptr<ui_mesh_data> make_triangle_data() {
    auto const vertex_data = ui::static_mesh_vertex_data::make_shared(3);
    auto const index_data = ui::static_mesh_index_data::make_shared(3);

    auto mesh_data = ui_mesh_data::make_shared(ui::primitive_type::triangle, vertex_data, index_data);

    vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        float const half_width = -5.0f;
        float const height = 10.0f;
        vertices[0].position = {0.0f, -height};
        vertices[1].position = {-half_width, 0.0f};
        vertices[2].position = {half_width, 0.0f};
    });

    index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
    });

    return mesh_data;
}

std::shared_ptr<ui_mesh_data> make_square_data() {
    auto const plane_data = ui::rect_plane_data::make_shared(1);
    plane_data->set_rect_position(ui::region{.origin = {0.0f, 0.0f}, .size = {1.0f, 1.0f}}, 0);

    return ui_mesh_data::make_shared(ui::primitive_type::triangle, plane_data->dynamic_vertex_data(),
                                     plane_data->dynamic_index_data());
}
}

ui_resource_lifetime::ui_resource_lifetime(std::shared_ptr<ui::standard> const &standard,
                                           ae::window_lifetime_id const &lifetime_id)
    : standard(standard),
      window_lifetime_id(lifetime_id),
      texture(ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look())),
      normal_font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          this->texture)),
      time_font_atlas(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold", .font_size = 26.0f, .words = " 1234567890.:+-"}, texture)),
      vertical_line_data(ui_resource_lifetime_utils::make_vertical_line_data()),
      triangle_data(ui_resource_lifetime_utils::make_triangle_data()),
      square_data(ui_resource_lifetime_utils::make_square_data()),
      display_space(display_space::make_shared(standard->view_look()->view_layout_guide()->region())),
      keyboard(ae::keyboard::make_shared(standard->event_manager())),
      base_lifecycle(std::make_shared<ae::ui_base_lifecycle>(lifetime_id)) {
}
