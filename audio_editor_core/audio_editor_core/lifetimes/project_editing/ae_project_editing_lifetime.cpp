//
//  ae_project_editing_lifetime.cpp
//

#include "ae_project_editing_lifetime.h"

#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_export_interactor.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_file_module_loader.h>
#include <audio_editor_core/ae_file_module_loading_state_holder.h>
#include <audio_editor_core/ae_grid_content_pool.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_horizontal_scrolling.h>
#include <audio_editor_core/ae_import_interactor.h>
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_editor.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_module_editor.h>
#include <audio_editor_core/ae_module_pool.h>
#include <audio_editor_core/ae_module_renaming_opener.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_project_editing_receiver.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_lifecycle.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_project_path.h>
#include <audio_editor_core/ae_reverter.h>
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_system_path.h>
#include <audio_editor_core/ae_time_editing_opener.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <audio_editor_core/ae_timing.h>
#include <audio_editor_core/ae_waveform_mesh_importer.h>
#include <audio_editor_core/ae_zooming_pair.h>

#include <audio_editor_core/ae_app_settings.hpp>
#include <audio_editor_core/ae_deselector.hpp>
#include <audio_editor_core/ae_display_space_range.hpp>
#include <audio_editor_core/ae_escaper.hpp>
#include <audio_editor_core/ae_grid_updater.hpp>
#include <audio_editor_core/ae_marker_renaming_opener.hpp>
#include <audio_editor_core/ae_marker_selector.hpp>
#include <audio_editor_core/ae_module_selector.hpp>
#include <audio_editor_core/ae_project_settings_opener.hpp>
#include <audio_editor_core/ae_range_selector.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editing_lifetime> project_editing_lifetime::make_shared(
    ae::project_lifetime_id const &lifetime_id) {
    auto const project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id);

    return std::make_shared<project_editing_lifetime>(project_lifetime.get(), hierarchy::app_lifetime().get());
}

project_editing_lifetime::project_editing_lifetime(project_lifetime const *project_lifetime,
                                                   app_lifetime const *app_lifetime)
    : project_lifetime_id(project_lifetime->lifetime_id),
      project_format(project_lifetime->project_format),
      module_content_pool(module_content_pool::make_shared()),
      pasting_module_content_pool(pasting_module_content_pool::make_shared()),
      marker_content_pool(marker_content_pool::make_shared()),
      pasting_marker_content_pool(pasting_marker_content_pool::make_shared()),
      grid_content_pool(grid_content_pool::make_shared()),
      action_sender(
          std::make_shared<ae::project_action_sender>(project_lifetime_id, app_lifetime->action_sender.get())),
      pinch_gesture_controller(std::make_shared<ae::pinch_gesture_controller>(project_lifetime->zooming_pair.get())),
      scroll_gesture_controller(std::make_shared<ae::scroll_gesture_controller>(
          project_lifetime->horizontal_scrolling.get(), project_lifetime->vertical_scrolling.get(),
          project_lifetime->zooming_pair.get())),
      database(database::make_shared(project_lifetime->project_path->db_file())),
      timing(std::make_shared<ae::timing>(project_format.sample_rate, app_lifetime->app_settings.get())),
      nudging(std::make_shared<ae::nudging>(this->timing.get(), app_lifetime->app_settings.get())),
      grid_updater(std::make_shared<ae::grid_updater>(this->timing.get(), this->nudging.get(),
                                                      app_lifetime->app_settings.get(), this->grid_content_pool.get())),
      module_pool(std::make_shared<ae::module_pool>(this->database.get())),
      waveforms_mesh_importer(
          waveform_mesh_importer::make_shared(project_lifetime->lifetime_id, this->module_pool.get())),
      selected_module_pool(std::make_shared<ae::selected_module_pool>()),
      marker_pool(std::make_shared<ae::marker_pool>(this->database.get())),
      selected_marker_pool(std::make_shared<ae::selected_marker_pool>()),
      pasteboard(std::make_shared<ae::pasteboard>()),
      exporter(std::make_shared<ae::exporter>()),
      editing_status(std::make_shared<ae::editing_status>(this->exporter.get())),
      deselector(std::make_shared<ae::deselector>(this->selected_module_pool.get(), this->selected_marker_pool.get())),
      module_selector(std::make_shared<ae::module_selector>(this->module_pool.get(), this->selected_module_pool.get(),
                                                            this->editing_status.get())),
      marker_selector(std::make_shared<ae::marker_selector>(this->marker_pool.get(), this->selected_marker_pool.get(),
                                                            this->editing_status.get(), this->deselector.get())),
      playing_toggler(std::make_shared<ae::playing_toggler>(project_lifetime->player.get())),
      modal_lifecycle(project_modal_lifecycle::make_shared(project_lifetime_id)),
      nudger(std::make_shared<ae::nudger>(project_lifetime->player.get(), this->nudging.get())),
      edge_holder(std::make_shared<ae::edge_holder>(this->database.get())),
      edge_editor(std::make_shared<ae::edge_editor>(this->edge_holder.get(), project_lifetime->player.get(),
                                                    this->editing_status.get())),
      jumper(std::make_shared<ae::jumper>(project_lifetime->player.get(), this->module_pool.get(),
                                          this->marker_pool.get(), this->edge_holder.get())),
      time_editing_opener(std::make_shared<ae::time_editing_opener>(project_lifetime->player.get(), this->timing.get(),
                                                                    this->modal_lifecycle.get())),
      marker_editor(std::make_shared<ae::marker_editor>(project_lifetime->player.get(), this->marker_pool.get(),
                                                        this->database.get(), this->editing_status.get(),
                                                        this->selected_marker_pool.get(), this->pasteboard.get())),
      module_renaming_opener(std::make_shared<ae::module_renaming_opener>(
          this->modal_lifecycle.get(), this->editing_status.get(), this->deselector.get())),
      marker_renaming_opener(std::make_shared<ae::marker_renaming_opener>(
          this->modal_lifecycle.get(), this->editing_status.get(), this->deselector.get())),
      settings_opener(std::make_shared<ae::project_settings_opener>(project_lifetime->lifetime_id,
                                                                    app_lifetime->project_settings_lifecycle.get())),
      export_interactor(std::make_shared<ae::export_interactor>(
          project_format, this->modal_lifecycle.get(), this->editing_status.get(), this->edge_holder.get(),
          project_lifetime->player.get(), this->exporter.get(), project_lifetime->timeline_holder.get())),
      timeline_updater(
          std::make_shared<ae::timeline_updater>(this->module_pool.get(), project_lifetime->timeline_holder.get())),
      reverter(std::make_shared<ae::reverter>(project_lifetime->project_path.get(), this->database.get(),
                                              this->module_pool.get(), this->marker_pool.get(), this->pasteboard.get(),
                                              this->edge_holder.get(), this->selected_module_pool.get(),
                                              this->editing_status.get())),
      file_module_loading_state_holder(std::make_shared<ae::file_module_loading_state_holder>()),
      file_module_loader(file_module_loader::make_shared(
          project_lifetime_id.guid, project_lifetime->project_path.get(), this->project_format,
          this->file_module_loading_state_holder.get(), this->database.get(), this->pasteboard.get())),
      import_interactor(std::make_shared<ae::import_interactor>(this->modal_lifecycle.get(), this->editing_status.get(),
                                                                this->file_module_loader.get())),
      module_editor(std::make_shared<ae::module_editor>(
          project_lifetime->player.get(), this->module_pool.get(), this->marker_pool.get(),
          this->selected_module_pool.get(), this->pasteboard.get(), this->database.get(), this->editing_status.get())),
      range_selector(std::make_shared<ae::range_selector>(project_lifetime->player.get(), this->deselector.get())),
      display_space_range(std::make_shared<ae::display_space_range>(
          project_lifetime->project_format, project_lifetime->display_space.get(), project_lifetime->player.get())),
      escaper(std::make_shared<ae::escaper>(this->pasteboard.get(), this->selected_module_pool.get(),
                                            this->selected_marker_pool.get())),
      receiver(std::make_shared<project_editing_receiver>(
          project_lifetime_id, this->database.get(), this->module_editor.get(), this->playing_toggler.get(),
          this->nudging.get(), this->nudger.get(), this->jumper.get(), this->edge_editor.get(),
          this->time_editing_opener.get(), this->marker_editor.get(), this->module_renaming_opener.get(),
          this->marker_renaming_opener.get(), this->settings_opener.get(), this->timing.get(),
          this->import_interactor.get(), this->export_interactor.get(), this->reverter.get(),
          this->module_selector.get(), this->marker_selector.get(), this->escaper.get(), this->pasteboard.get())) {
}
