//
//  ae_ui_root_lifetime.mm
//

#include "ae_ui_root_lifetime.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_mesh_data.h>
#include <audio_editor_core/ae_ui_modal_bg.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_root.h>
#include <audio_editor_core/ae_ui_scroller.h>
#include <audio_editor_core/ae_ui_time.h>
#include <audio_editor_core/ae_ui_track.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_lifetime> ui_root_lifetime::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                                ae::window_lifetime_id const &window_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(window_lifetime_id);
    auto const &waveforms_mesh_importer = project_lifetime->waveforms_mesh_importer;

    auto const texture = ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look());
    auto const font_atlas_14 =
        ui::font_atlas::make_shared({.font_name = "TrebuchetMS-Bold",
                                     .font_size = 14.0f,
                                     .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
                                    texture);
    auto const vertical_line_data =
        ui_mesh_data::make_shared(ui::primitive_type::line, ui::static_mesh_vertex_data::make_shared(2),
                                  ui::static_mesh_index_data::make_shared(2));
    auto const display_space = display_space::make_shared(standard->view_look()->view_layout_guide()->region());
    auto const waveforms = ui_module_waveforms::make_shared(
        window_lifetime_id, standard, project_lifetime->module_location_pool, waveforms_mesh_importer);
    auto const modules = ui_modules::make_shared(window_lifetime_id, display_space, standard, font_atlas_14,
                                                 project_lifetime->module_location_pool, waveforms);
    auto const markers =
        ui_markers::make_shared(window_lifetime_id, display_space, project_lifetime->marker_location_pool, standard,
                                font_atlas_14, vertical_line_data);

    return std::make_shared<ui_root_lifetime>(standard, window_lifetime_id, texture, font_atlas_14, vertical_line_data,
                                              display_space, waveforms, modules, markers);
}

ui_root_lifetime::ui_root_lifetime(
    std::shared_ptr<ui::standard> const &standard, ae::window_lifetime_id const &window_lifetime_id,
    std::shared_ptr<ui::texture> const &texture, std::shared_ptr<ui::font_atlas> const &font_atlas_14,
    std::shared_ptr<ui_mesh_data> const &vertical_line_data, std::shared_ptr<ae::display_space> const &display_space,
    std::shared_ptr<ui_module_waveforms> const &waveforms, std::shared_ptr<ui_modules> const &modules,
    std::shared_ptr<ui_markers> const &markers)
    : window_lifetime_id(window_lifetime_id),
      standard(standard),
      texture(texture),
      font_atlas_14(font_atlas_14),
      vertical_line_data(vertical_line_data),
      display_space(display_space),
      keyboard(ae::keyboard::make_shared(standard->event_manager())),
      waveforms(waveforms),
      modules(modules),
      edge(ui_edge::make_shared(window_lifetime_id, this->vertical_line_data, this->display_space, this->standard,
                                this->font_atlas_14)),
      markers(markers),
      track(ui_track::make_shared(window_lifetime_id, this->standard, this->display_space, this->modules)),
      scroller(ui_scroller::make_shared(window_lifetime_id, this->standard, this->track, this->edge, this->markers)),
      modal_bg(ui_modal_bg::make_shared(window_lifetime_id, this->standard)),
      time(ui_time::make_shared(window_lifetime_id, this->standard, this->texture)),
      editing_root(ui_editing_root::make_shared(window_lifetime_id, this->standard, this->font_atlas_14, this->keyboard,
                                                this->scroller, this->modal_bg, this->time)),
      root(ui_root::make_shared(standard, window_lifetime_id, this->editing_root)) {
    this->vertical_line_data->vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        vertices.at(0).position = {0.0f, -0.5f};
        vertices.at(1).position = {0.0f, 0.5f};
    });

    this->vertical_line_data->index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices.at(0) = 0;
        indices.at(1) = 1;
    });
}
