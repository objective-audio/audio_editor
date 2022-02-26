//
//  ae_project_editor_level.cpp
//

#include "ae_project_editor_level.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_time_editor_maker.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level> project_editor_level::make_shared(std::string const &identifier,
                                                                        ae::file_info const &file_info) {
    auto const &project_level = hierarchy::project_level_for_id(identifier);
    return std::shared_ptr<project_editor_level>(
        new project_editor_level{identifier, file_info, project_level->project_url});
}

project_editor_level::project_editor_level(std::string const &identifier, ae::file_info const &file_info,
                                           std::shared_ptr<project_url> const &project_url)
    : identifier(identifier),
      file_info(file_info),
      timing(timing::make_shared(file_info.sample_rate)),
      nudging(nudging::make_shared(this->timing)),
      file_track(file_track::make_shared()),
      marker_pool(marker_pool::make_shared()),
      edge_editor(edge_editor::make_shared()),
      pasteboard(pasteboard::make_shared()),
      database(database::make_shared(project_url->db_file())),
      exporter(exporter::make_shared()),
      time_editor_maker(time_editor_maker::make_shared()),
      editor(project_editor::make_shared(identifier, file_info, this->nudging, this->timing)) {
}
