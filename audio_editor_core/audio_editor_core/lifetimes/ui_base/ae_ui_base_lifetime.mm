//
//  ae_ui_base_lifetime.cpp
//

#include "ae_ui_base_lifetime.hpp"
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_lifetime.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_modal_bg.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_root.h>
#include <audio_editor_core/ae_ui_scroller.h>
#include <audio_editor_core/ae_ui_time.h>
#include <audio_editor_core/ae_ui_track.h>
#include <audio_editor_core/ae_ui_resource_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

ui_base_lifetime::ui_base_lifetime(ae::window_lifetime_id const &lifetime_id)
    : ui_base_lifetime(lifetime_id, hierarchy::project_lifetime_for_id(lifetime_id).get(),
                       ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id).get()) {
}

ui_base_lifetime::ui_base_lifetime(ae::window_lifetime_id const &lifetime_id, project_lifetime *project_lifetime,
                                   ui_resource_lifetime *resource_lifetime)
    : window_lifetime_id(lifetime_id),
      waveforms(ui_module_waveforms::make_shared(lifetime_id, resource_lifetime->standard,
                                                 project_lifetime->module_location_pool,
                                                 project_lifetime->waveforms_mesh_importer)),
      modules(ui_modules::make_shared(lifetime_id, resource_lifetime->display_space, resource_lifetime->standard,
                                      resource_lifetime->font_atlas_14, project_lifetime->module_location_pool,
                                      this->waveforms)),
      edge(ui_edge::make_shared(window_lifetime_id, resource_lifetime->vertical_line_data,
                                resource_lifetime->display_space, resource_lifetime->standard,
                                resource_lifetime->font_atlas_14)),
      markers(ui_markers::make_shared(lifetime_id, resource_lifetime->display_space,
                                      project_lifetime->marker_location_pool, resource_lifetime->standard,
                                      resource_lifetime->font_atlas_14, resource_lifetime->vertical_line_data)),
      track(ui_track::make_shared(lifetime_id, resource_lifetime->standard, resource_lifetime->display_space,
                                  this->modules)),
      scroller(
          ui_scroller::make_shared(lifetime_id, resource_lifetime->standard, this->track, this->edge, this->markers)),
      modal_bg(ui_modal_bg::make_shared(lifetime_id, resource_lifetime->standard)),
      time(ui_time::make_shared(window_lifetime_id, resource_lifetime->standard, resource_lifetime->texture)),
      editing_root(ui_editing_root::make_shared(lifetime_id, resource_lifetime->standard,
                                                resource_lifetime->font_atlas_14, resource_lifetime->keyboard,
                                                this->scroller, this->modal_bg, this->time)),
      root(ui_root::make_shared(resource_lifetime->standard, lifetime_id, this->editing_root)) {
}
