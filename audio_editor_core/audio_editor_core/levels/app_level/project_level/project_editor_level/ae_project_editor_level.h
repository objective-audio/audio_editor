//
//  ae_project_editor_level.h
//

#pragma once

#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>

#include <memory>
#include <string>

namespace yas::ae {
class file_info;
class project_url;
class timing;
class nudge_settings;
class file_track;
class marker_pool;
class pasteboard;
class database;
class exporter;
class playing_toggler;
class time_editor_level_router;
class timeline_updater;
class nudger;
class jumper;
class edge_holder;
class edge_editor;
class project_editor;
class project_editor_responder;
class project_editor_launcher;
class time_editor_launcher;
class marker_editor;

struct project_editor_level final {
    [[nodiscard]] static std::shared_ptr<project_editor_level> make_shared(ae::project_id const &project_id,
                                                                           file_info const &);

    identifier const instance_id;
    ae::project_id const project_id;
    file_info const file_info;

    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudge_settings> const nudge_settings;
    std::shared_ptr<file_track> const file_track;
    std::shared_ptr<marker_pool> const marker_pool;
    std::shared_ptr<pasteboard> const pasteboard;
    std::shared_ptr<database> const database;
    std::shared_ptr<exporter> const exporter;
    std::shared_ptr<playing_toggler> const playing_toggler;
    std::shared_ptr<time_editor_level_router> const time_editor_level_router;
    std::shared_ptr<timeline_updater> const timeline_updater;
    std::shared_ptr<nudger> const nudger;
    std::shared_ptr<edge_holder> const edge_holder;
    std::shared_ptr<edge_editor> const edge_editor;
    std::shared_ptr<jumper> const jumper;
    std::shared_ptr<project_editor_launcher> const launcher;
    std::shared_ptr<time_editor_launcher> const time_editor_launcher;
    std::shared_ptr<marker_editor> const marker_editor;
    std::shared_ptr<project_editor> const editor;
    std::shared_ptr<project_editor_responder> const responder;

   private:
    project_editor_level(ae::project_id const &identifier, ae::file_info const &, std::shared_ptr<project_url> const &);
};
}  // namespace yas::ae
