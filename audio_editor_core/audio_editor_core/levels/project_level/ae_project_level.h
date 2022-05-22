//
//  ae_project_level.h
//

#pragma once

#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>
#include <cpp_utils/yas_url.h>

#include <memory>
#include <string>

namespace yas::ae {
class app_level;
class project_url;
class zooming_pair;
class scrolling;
class system_url;
class player;
class responder_stack;
class project_state_holder;
class project_closer;
class project_launcher;
class timing;
class nudge_settings;
class file_track;
class marker_pool;
class pasteboard;
class database;
class exporter;
class editing_status;
class playing_toggler;
class project_sub_level_router;
class timeline_holder;
class nudger;
class edge_holder;
class edge_editor;
class jumper;
class time_editor_launcher;
class marker_editor;
class module_renaming_launcher;
class export_interactor;
class database_updater;
class timeline_updater;
class reverter;
class file_module_loading_state_holder;
class file_module_loader;
class track_editor;
class project_editor_responder;
class project_editor_launcher;
class action_controller;

struct project_level final {
    [[nodiscard]] static std::shared_ptr<project_level> make_shared(project_id const &, project_format const &,
                                                                    url const &file_url);

    project_level(ae::project_id const &, project_format const &, url const &file_url,
                  std::shared_ptr<app_level> const &);

    identifier const instance_id;
    ae::project_id const project_id;
    project_format const project_format;
    url const file_url;

    std::shared_ptr<ae::project_url> const project_url;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<ae::responder_stack> const responder_stack;
    std::shared_ptr<ae::project_state_holder> const state_holder;
    std::shared_ptr<ae::project_closer> const closer;
    std::shared_ptr<ae::action_controller> const action_controller;

    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudge_settings> const nudge_settings;
    std::shared_ptr<file_track> const file_track;
    std::shared_ptr<marker_pool> const marker_pool;
    std::shared_ptr<pasteboard> const pasteboard;
    std::shared_ptr<database> const database;
    std::shared_ptr<exporter> const exporter;
    std::shared_ptr<editing_status> const editing_status;
    std::shared_ptr<playing_toggler> const playing_toggler;
    std::shared_ptr<project_sub_level_router> const sub_level_router;
    std::shared_ptr<timeline_holder> const timeline_holder;
    std::shared_ptr<nudger> const nudger;
    std::shared_ptr<edge_holder> const edge_holder;
    std::shared_ptr<edge_editor> const edge_editor;
    std::shared_ptr<jumper> const jumper;
    std::shared_ptr<time_editor_launcher> const time_editor_launcher;
    std::shared_ptr<marker_editor> const marker_editor;
    std::shared_ptr<module_renaming_launcher> const module_renaming_launcher;
    std::shared_ptr<export_interactor> const export_interactor;
    std::shared_ptr<database_updater> const database_updater;
    std::shared_ptr<timeline_updater> const timeline_updater;
    std::shared_ptr<reverter> const reverter;
    std::shared_ptr<file_module_loading_state_holder> const file_module_loading_state_holder;
    std::shared_ptr<file_module_loader> const file_module_loader;
    std::shared_ptr<track_editor> const track_editor;
    std::shared_ptr<project_editor_launcher> const editor_launcher;
    std::shared_ptr<project_editor_responder> const responder;

    std::shared_ptr<ae::project_launcher> const launcher;
};
}  // namespace yas::ae
