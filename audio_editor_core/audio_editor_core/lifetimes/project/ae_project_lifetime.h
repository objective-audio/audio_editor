//
//  ae_project_lifetime.h
//

#pragma once

#include <audio_editor_core/ae_grid_content_pool.h>
#include <audio_editor_core/ae_marker_content_pool.h>
#include <audio_editor_core/ae_module_content_pool.h>
#include <audio_editor_core/ae_pasting_marker_content_pool.h>
#include <audio_editor_core/ae_pasting_module_content_pool.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <audio_editor_core/ae_selected_marker_pool.hpp>
#include <audio_editor_core/ae_selected_module_pool.hpp>
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
class time_editor_opener;
class marker_editor;
class module_renaming_opener;
class marker_renaming_opener;
class settings_opener;
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
class project_receiver;
class window_lifetime;
class display_space_range;

struct project_lifetime final {
    [[nodiscard]] static std::shared_ptr<project_lifetime> make_shared(window_lifetime_id const &);

    project_lifetime(window_lifetime const *, app_lifetime const *);

    window_lifetime_id const window_lifetime_id;
    project_format const project_format;

    std::shared_ptr<module_content_pool> const module_content_pool;
    std::shared_ptr<pasting_module_content_pool> const pasting_module_content_pool;
    std::shared_ptr<marker_content_pool> const marker_content_pool;
    std::shared_ptr<pasting_marker_content_pool> const pasting_marker_content_pool;
    std::shared_ptr<grid_content_pool> const grid_content_pool;
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
    std::shared_ptr<pasteboard> const pasteboard;
    std::shared_ptr<exporter> const exporter;
    std::shared_ptr<editing_status> const editing_status;
    std::shared_ptr<deselector> const deselector;
    std::shared_ptr<module_selector> const module_selector;
    std::shared_ptr<marker_selector> const marker_selector;
    std::shared_ptr<playing_toggler> const playing_toggler;
    std::shared_ptr<project_modal_lifecycle> const modal_lifecycle;
    std::shared_ptr<nudger> const nudger;
    std::shared_ptr<edge_holder> const edge_holder;
    std::shared_ptr<edge_editor> const edge_editor;
    std::shared_ptr<jumper> const jumper;
    std::shared_ptr<time_editor_opener> const time_editor_opener;
    std::shared_ptr<marker_editor> const marker_editor;
    std::shared_ptr<module_renaming_opener> const module_renaming_opener;
    std::shared_ptr<marker_renaming_opener> const marker_renaming_opener;
    std::shared_ptr<settings_opener> const settings_opener;
    std::shared_ptr<export_interactor> const export_interactor;
    std::shared_ptr<timeline_updater> const timeline_updater;
    std::shared_ptr<reverter> const reverter;
    std::shared_ptr<file_module_loading_state_holder> const file_module_loading_state_holder;
    std::shared_ptr<file_module_loader> const file_module_loader;
    std::shared_ptr<import_interactor> const import_interactor;
    std::shared_ptr<module_editor> const module_editor;
    std::shared_ptr<range_selector> const range_selector;
    std::shared_ptr<display_space_range> const display_space_range;
    std::shared_ptr<escaper> const escaper;
    std::shared_ptr<project_receiver> const receiver;

   private:
    project_lifetime(project_lifetime const &) = delete;
    project_lifetime(project_lifetime &&) = delete;
    project_lifetime &operator=(project_lifetime const &) = delete;
    project_lifetime &operator=(project_lifetime &&) = delete;
};
}  // namespace yas::ae
