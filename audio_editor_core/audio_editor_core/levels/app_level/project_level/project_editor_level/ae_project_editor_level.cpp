//
//  ae_project_editor_level.cpp
//

#include "ae_project_editor_level.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_editor_launcher.h>
#include <audio_editor_core/ae_project_editor_responder.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_time_editor_level_router.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level> project_editor_level::make_shared(std::string const &project_id,
                                                                        ae::file_info const &file_info) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<project_editor_level>(
        new project_editor_level{project_id, file_info, project_level->project_url});
}

project_editor_level::project_editor_level(std::string const &project_id, ae::file_info const &file_info,
                                           std::shared_ptr<project_url> const &project_url)
    : project_id(project_id),
      file_info(file_info),
      timing(timing::make_shared(file_info.sample_rate)),
      nudge_settings(nudge_settings::make_shared(this->timing)),
      file_track(file_track::make_shared()),
      marker_pool(marker_pool::make_shared()),
      pasteboard(pasteboard::make_shared()),
      database(database::make_shared(project_url->db_file())),
      exporter(exporter::make_shared()),
      playing_toggler(playing_toggler::make_shared(project_id)),
      time_editor_level_router(time_editor_level_router::make_shared(project_id)),
      timeline_updater(timeline_updater::make_shared(project_id, file_info)),
      nudger(nudger::make_shared(project_id, this->nudge_settings)),
      edge_holder(edge_holder::make_shared()),
      edge_editor(edge_editor::make_shared(project_id, this->edge_holder, this->exporter)),
      jumper(jumper::make_shared(project_id, this->file_track, this->marker_pool, this->edge_holder)),
      launcher(project_editor_launcher::make_shared(project_id, file_info, this->timeline_updater, this->database,
                                                    this->file_track, this->edge_holder)),
      editor(project_editor::make_shared(project_id, file_info, this->file_track, this->marker_pool, this->edge_holder,
                                         this->pasteboard, this->database, this->exporter, this->timing,
                                         this->time_editor_level_router, this->timeline_updater)),
      responder(project_editor_responder::make_shared(this->editor, this->playing_toggler, this->nudge_settings,
                                                      this->nudger, this->jumper, this->edge_editor)) {
    this->launcher->launch();
}
