//
//  project_editing_lifetime.h
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/project_editing/features/grid_content_pool.h>
#include <ae-core/project_editing/features/marker_content_pool.h>
#include <ae-core/project_editing/features/module_content_pool.h>
#include <ae-core/project_editing/features/pasting_marker_content_pool.h>
#include <ae-core/project_editing/features/pasting_module_content_pool.h>
#include <ae-core/project_editing/features/track_content_pool.h>

#include <ae-core/project_editing/features/selected_marker_pool.hpp>
#include <ae-core/project_editing/features/selected_module_pool.hpp>
#include <ae-core/project_editing/features/selected_track_pool.hpp>
#include <memory>
#include <string>

namespace yas::ae {
class app_lifetime;
class timing;
class nudging;
class grid_updater;
class module_pool;
class module_selector;
class marker_pool;
class deselector;
class marker_selector;
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
class time_editing_opener;
class marker_editor;
class module_renaming_opener;
class marker_renaming_opener;
class project_settings_opener;
class import_interactor;
class export_interactor;
class timeline_updater;
class reverter;
class file_module_loading_state_holder;
class file_module_loader;
class module_editor;
class project_action_sender;
class pinch_gesture_controller;
class scroll_gesture_controller;
class waveform_mesh_importer;
class range_selector;
class escaper;
class project_editing_receiver;
class project_lifetime;
class display_space_time_range;
class display_space_track_range;
class display_space_range;
class track_selector;
class selector_enabler;
class track_jumper;

struct project_editing_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_editing_lifetime> make_shared(project_lifetime_id const &);

    project_editing_lifetime(project_lifetime const *, app_lifetime const *);

    project_lifetime_id const project_lifetime_id;
    project_format const project_format;

    std::shared_ptr<module_content_pool> const module_content_pool;
    std::shared_ptr<pasting_module_content_pool> const pasting_module_content_pool;
    std::shared_ptr<marker_content_pool> const marker_content_pool;
    std::shared_ptr<pasting_marker_content_pool> const pasting_marker_content_pool;
    std::shared_ptr<grid_content_pool> const grid_content_pool;
    std::shared_ptr<track_content_pool> const track_content_pool;
    std::shared_ptr<project_action_sender> const action_sender;
    std::shared_ptr<pinch_gesture_controller> const pinch_gesture_controller;
    std::shared_ptr<scroll_gesture_controller> const scroll_gesture_controller;

    std::shared_ptr<database> const database;
    std::shared_ptr<timing> const timing;
    std::shared_ptr<nudging> const nudging;
    std::shared_ptr<grid_updater> const grid_updater;
    std::shared_ptr<module_pool> const module_pool;
    std::shared_ptr<waveform_mesh_importer> const waveforms_mesh_importer;
    std::shared_ptr<selected_module_pool> const selected_module_pool;
    std::shared_ptr<marker_pool> const marker_pool;
    std::shared_ptr<selected_marker_pool> const selected_marker_pool;
    std::shared_ptr<selected_track_pool> const selected_track_pool;
    std::shared_ptr<pasteboard> const pasteboard;
    std::shared_ptr<exporter> const exporter;
    std::shared_ptr<editing_status> const editing_status;
    std::shared_ptr<deselector> const deselector;
    std::shared_ptr<selector_enabler> const selector_enabler;
    std::shared_ptr<track_selector> const track_selector;
    std::shared_ptr<module_selector> const module_selector;
    std::shared_ptr<marker_selector> const marker_selector;
    std::shared_ptr<playing_toggler> const playing_toggler;
    std::shared_ptr<project_modal_lifecycle> const modal_lifecycle;
    std::shared_ptr<nudger> const nudger;
    std::shared_ptr<edge_holder> const edge_holder;
    std::shared_ptr<edge_editor> const edge_editor;
    std::shared_ptr<jumper> const jumper;
    std::shared_ptr<time_editing_opener> const time_editing_opener;
    std::shared_ptr<marker_editor> const marker_editor;
    std::shared_ptr<module_renaming_opener> const module_renaming_opener;
    std::shared_ptr<marker_renaming_opener> const marker_renaming_opener;
    std::shared_ptr<project_settings_opener> const settings_opener;
    std::shared_ptr<export_interactor> const export_interactor;
    std::shared_ptr<timeline_updater> const timeline_updater;
    std::shared_ptr<reverter> const reverter;
    std::shared_ptr<file_module_loading_state_holder> const file_module_loading_state_holder;
    std::shared_ptr<file_module_loader> const file_module_loader;
    std::shared_ptr<import_interactor> const import_interactor;
    std::shared_ptr<module_editor> const module_editor;
    std::shared_ptr<range_selector> const range_selector;
    std::shared_ptr<display_space_time_range> const display_space_time_range;
    std::shared_ptr<display_space_track_range> const display_space_track_range;
    std::shared_ptr<display_space_range> const display_space_range;
    std::shared_ptr<track_jumper> const track_jumper;
    std::shared_ptr<escaper> const escaper;
    std::shared_ptr<project_editing_receiver> const receiver;

   private:
    project_editing_lifetime(project_editing_lifetime const &) = delete;
    project_editing_lifetime(project_editing_lifetime &&) = delete;
    project_editing_lifetime &operator=(project_editing_lifetime const &) = delete;
    project_editing_lifetime &operator=(project_editing_lifetime &&) = delete;
};
}  // namespace yas::ae
