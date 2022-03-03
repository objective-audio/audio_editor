//
//  ae_ui_editing_root_level.mm
//

#include "ae_ui_editing_root_level.h"
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_ui_edge.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_markers.h>
#include <audio_editor_core/ae_ui_module_waveforms.h>
#include <audio_editor_core/ae_ui_modules.h>
#include <audio_editor_core/ae_ui_time.h>
#include <audio_editor_core/ae_ui_track.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_editing_root_level> ui_editing_root_level::make_shared(ui_project_id const &project_id) {
    return std::shared_ptr<ui_editing_root_level>(new ui_editing_root_level{project_id});
}

ui_editing_root_level::ui_editing_root_level(ui_project_id const &project_id)
    : location_pool(module_location_pool::make_shared()),
      waveforms(ui_module_waveforms::make_shared(project_id, this->location_pool)),
      modules(ui_modules::make_shared(project_id, this->location_pool, this->waveforms)),
      edge(ui_edge::make_shared(project_id)),
      markers(ui_markers::make_shared(project_id)),
      track(ui_track::make_shared(project_id, this->modules, this->edge, this->markers)),
      time(ui_time::make_shared(project_id)),
      editing_root(ui_editing_root::make_shared(project_id, this->track, this->time)) {
}
