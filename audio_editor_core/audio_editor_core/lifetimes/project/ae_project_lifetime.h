//
//  ae_project_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_marker_location_pool.h>
#include <audio_editor_core/ae_module_location_pool.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_id.h>
#include <cpp_utils/yas_identifier.h>
#include <cpp_utils/yas_url.h>

#include <memory>
#include <string>

namespace yas::ae {
class app_lifetime;
class project_url;
class zooming_pair;
class scrolling;
class system_url;
class player;
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
class project_modal_lifecycle;
class timeline_holder;
class nudger;
class edge_holder;
class edge_editor;
class jumper;
class time_editor_launcher;
class marker_editor;
class module_renaming_launcher;
class import_interactor;
class export_interactor;
class database_updater;
class timeline_updater;
class reverter;
class file_module_loading_state_holder;
class file_module_loader;
class track_editor;
class project_editor_responder;
class project_action_controller;
class pinch_gesture_controller;
class scroll_gesture_controller;
class waveform_mesh_importer;
class project_receiver;

struct project_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_lifetime> make_shared(project_id const &);
    [[nodiscard]] static std::shared_ptr<project_lifetime> make_shared(project_id const &, project_format const &,
                                                                       url const &project_dir_url);

    project_lifetime(ae::project_id const &, project_format const &, url const &project_dir_url,
                     std::shared_ptr<app_lifetime> const &);

    identifier const instance_id;
    ae::project_id const project_id;
    project_format const project_format;

    std::shared_ptr<ae::project_url> const project_url;
    std::shared_ptr<ae::zooming_pair> const zooming_pair;
    std::shared_ptr<ae::scrolling> const scrolling;
    std::shared_ptr<ae::player> const player;
    std::shared_ptr<ae::project_state_holder> const state_holder;
    std::shared_ptr<ae::project_closer> const closer;
    std::shared_ptr<module_location_pool> const module_location_pool;
    std::shared_ptr<marker_location_pool> const marker_location_pool;
    std::shared_ptr<waveform_mesh_importer> const waveforms_mesh_importer;
    std::shared_ptr<ae::project_action_controller> const action_controller;
    std::shared_ptr<ae::pinch_gesture_controller> const pinch_gesture_controller;
    std::shared_ptr<scroll_gesture_controller> const scroll_gesture_controller;

    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudge_settings> const nudge_settings;
    std::shared_ptr<file_track> const file_track;
    std::shared_ptr<marker_pool> const marker_pool;
    std::shared_ptr<pasteboard> const pasteboard;
    std::shared_ptr<database> const database;
    std::shared_ptr<exporter> const exporter;
    std::shared_ptr<editing_status> const editing_status;
    std::shared_ptr<playing_toggler> const playing_toggler;
    std::shared_ptr<project_modal_lifecycle> const modal_lifecycle;
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
    std::shared_ptr<import_interactor> const import_interactor;
    std::shared_ptr<track_editor> const track_editor;
    std::shared_ptr<project_editor_responder> const responder;
    std::shared_ptr<project_receiver> const receiver;

    std::shared_ptr<ae::project_launcher> const launcher;
};
}  // namespace yas::ae
