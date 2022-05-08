//
//  ae_project_editor_level.cpp
//

#include "ae_project_editor_level.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_export_interactor.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_editor.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_module_renaming_launcher.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_editor_launcher.h>
#include <audio_editor_core/ae_project_editor_responder.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_time_editor_launcher.h>
#include <audio_editor_core/ae_time_editor_level_router.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level> project_editor_level::make_shared(ae::project_id const &project_id,
                                                                        ae::file_info const &file_info) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<project_editor_level>(
        new project_editor_level{project_id, file_info, project_level->project_url});
}

project_editor_level::project_editor_level(ae::project_id const &project_id, ae::file_info const &file_info,
                                           std::shared_ptr<project_url> const &project_url)
    : project_id(project_id),
      file_info(file_info),
      timing(timing::make_shared(file_info.sample_rate)),
      nudge_settings(nudge_settings::make_shared(this->timing.get())),
      file_track(file_track::make_shared()),
      marker_pool(marker_pool::make_shared()),
      pasteboard(pasteboard::make_shared()),
      database(database::make_shared(project_url->db_file())),
      exporter(exporter::make_shared()),
      editing_status(editing_status::make_shared(this->exporter.get())),
      playing_toggler(playing_toggler::make_shared(project_id)),
      time_editor_level_router(time_editor_level_router::make_shared(project_id)),
      timeline_updater(timeline_holder::make_shared(project_id, file_info)),
      nudger(nudger::make_shared(project_id, this->nudge_settings.get())),
      edge_holder(edge_holder::make_shared()),
      edge_editor(edge_editor::make_shared(project_id, this->edge_holder.get(), this->editing_status.get())),
      jumper(jumper::make_shared(project_id, this->file_track.get(), this->marker_pool.get(), this->edge_holder.get())),
      launcher(project_editor_launcher::make_shared(project_id, file_info, this->timeline_updater.get(),
                                                    this->database.get(), this->file_track.get(),
                                                    this->edge_holder.get())),
      time_editor_launcher(
          time_editor_launcher::make_shared(project_id, this->timing.get(), this->time_editor_level_router.get())),
      marker_editor(marker_editor::make_shared(project_id, this->marker_pool.get(), this->database.get(),
                                               this->editing_status.get())),
      module_renaming_launcher(module_renaming_launcher::make_shared(project_id, this->editing_status.get())),
      export_interactor(export_interactor::make_shared(project_id, file_info, this->editing_status.get(),
                                                       this->edge_holder.get(), this->exporter.get(),
                                                       this->timeline_updater.get())),
      editor(project_editor::make_shared(project_id, file_info, this->file_track.get(), this->marker_pool.get(),
                                         this->edge_holder.get(), this->pasteboard.get(), this->database.get(),
                                         this->timeline_updater.get(), this->editing_status.get())),
      responder(project_editor_responder::make_shared(
          this->editor.get(), this->playing_toggler.get(), this->nudge_settings.get(), this->nudger.get(),
          this->jumper.get(), this->edge_editor.get(), this->time_editor_launcher.get(), this->marker_editor.get(),
          this->module_renaming_launcher.get(), this->timing.get(), this->export_interactor.get())) {
    this->launcher->launch();
}
