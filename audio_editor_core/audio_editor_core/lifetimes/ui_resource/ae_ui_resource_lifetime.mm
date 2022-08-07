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
    auto mesh_data = ui_mesh_data::make_shared(ui::primitive_type::line, ui::static_mesh_vertex_data::make_shared(2),
                                               ui::static_mesh_index_data::make_shared(2));
    mesh_data->vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        vertices.at(0).position = {0.0f, -0.5f};
        vertices.at(1).position = {0.0f, 0.5f};
    });
    mesh_data->index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices.at(0) = 0;
        indices.at(1) = 1;
    });
    return mesh_data;
}
}

ui_resource_lifetime::ui_resource_lifetime(std::shared_ptr<ui::standard> const &standard,
                                           ae::window_lifetime_id const &lifetime_id)
    : standard(standard),
      window_lifetime_id(lifetime_id),
      texture(ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look())),
      font_atlas_14(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          this->texture)),
      vertical_line_data(ui_resource_lifetime_utils::make_vertical_line_data()),
      display_space(display_space::make_shared(standard->view_look()->view_layout_guide()->region())),
      keyboard(ae::keyboard::make_shared(standard->event_manager())),
      base_lifecycle(std::make_shared<ae::ui_base_lifecycle>(lifetime_id)) {
}
