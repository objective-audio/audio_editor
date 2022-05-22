//
//  ae_ui_root_level.mm
//

#include "ae_ui_root_level.h"
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
#include <audio_editor_core/ae_waveform_mesh_importer.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level> ui_root_level::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                          ui_project_id const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id.project_id);
    auto const url = project_level->project_url->editing_file();
    return std::shared_ptr<ui_root_level>(
        new ui_root_level{standard, project_id, waveform_mesh_importer::make_shared(url)});
}

ui_root_level::ui_root_level(std::shared_ptr<ui::standard> const &standard, ui_project_id const &project_id,
                             std::shared_ptr<waveform_mesh_importer> const &waveform_mesh_importer)
    : project_id(project_id),
      standard(standard),
      texture(ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look())),
      font_atlas_14(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          this->texture)),
      vertical_line_data(ui_mesh_data::make_shared(ui::primitive_type::line,
                                                   ui::static_mesh_vertex_data::make_shared(2),
                                                   ui::static_mesh_index_data::make_shared(2))),
      display_space(display_space::make_shared(standard->view_look()->view_layout_guide()->region())),
      keyboard(ae::keyboard::make_shared(standard->event_manager())),
      module_location_pool(module_location_pool::make_shared()),
      marker_location_pool(marker_location_pool::make_shared()),
      waveforms_mesh_importer(waveform_mesh_importer),
      waveforms(ui_module_waveforms::make_shared(project_id, this->standard, this->module_location_pool,
                                                 this->waveforms_mesh_importer)),
      modules(ui_modules::make_shared(project_id, this->display_space, this->standard, this->font_atlas_14,
                                      this->module_location_pool, this->waveforms)),
      edge(ui_edge::make_shared(project_id, this->vertical_line_data, this->display_space, this->standard,
                                this->font_atlas_14)),
      markers(ui_markers::make_shared(project_id, this->display_space, this->marker_location_pool, this->standard,
                                      this->font_atlas_14, this->vertical_line_data)),
      track(ui_track::make_shared(project_id, this->standard, this->display_space, this->modules)),
      scroller(ui_scroller::make_shared(project_id, this->standard, this->track, this->edge, this->markers)),
      modal_bg(ui_modal_bg::make_shared(project_id, this->standard)),
      time(ui_time::make_shared(project_id, this->standard, this->texture)),
      editing_root(ui_editing_root::make_shared(project_id, this->standard, this->font_atlas_14, this->keyboard,
                                                this->scroller, this->modal_bg, this->time)),
      root(ui_root::make_shared(standard, project_id, this->editing_root)) {
    this->vertical_line_data->vertex_data->write_once([](std::vector<ui::vertex2d_t> &vertices) {
        vertices.at(0).position = {0.0f, -0.5f};
        vertices.at(1).position = {0.0f, 0.5f};
    });

    this->vertical_line_data->index_data->write_once([](std::vector<ui::index2d_t> &indices) {
        indices.at(0) = 0;
        indices.at(1) = 1;
    });
}
