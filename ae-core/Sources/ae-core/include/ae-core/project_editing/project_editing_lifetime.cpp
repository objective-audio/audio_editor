//
//  project_editing_lifetime.cpp
//

#include "project_editing_lifetime.h"

#include <ae-core/app/app_lifetime.h>
#include <ae-core/app/features/file_importer.h>
#include <ae-core/app/features/file_info_loader.h>
#include <ae-core/app/features/system_path.h>
#include <ae-core/app/lifecycles/project_lifecycle.h>
#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/horizontal_scrolling.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project/features/project_path.h>
#include <ae-core/project/features/timeline_holder.h>
#include <ae-core/project/features/zooming_pair.h>
#include <ae-core/project_editing/features/database.h>
#include <ae-core/project_editing/features/edge_editor.h>
#include <ae-core/project_editing/features/edge_holder.h>
#include <ae-core/project_editing/features/editing_status.h>
#include <ae-core/project_editing/features/export_interactor.h>
#include <ae-core/project_editing/features/exporter.h>
#include <ae-core/project_editing/features/file_module_loader.h>
#include <ae-core/project_editing/features/file_module_loading_state_holder.h>
#include <ae-core/project_editing/features/grid_content_pool.h>
#include <ae-core/project_editing/features/import_interactor.h>
#include <ae-core/project_editing/features/jumper.h>
#include <ae-core/project_editing/features/marker_editor.h>
#include <ae-core/project_editing/features/marker_pool.h>
#include <ae-core/project_editing/features/module_editor.h>
#include <ae-core/project_editing/features/module_pool.h>
#include <ae-core/project_editing/features/module_renaming_opener.h>
#include <ae-core/project_editing/features/nudger.h>
#include <ae-core/project_editing/features/nudging.h>
#include <ae-core/project_editing/features/pasteboard.h>
#include <ae-core/project_editing/features/pinch_gesture_controller.h>
#include <ae-core/project_editing/features/playing_toggler.h>
#include <ae-core/project_editing/features/project_action_sender.h>
#include <ae-core/project_editing/features/project_editing_receiver.h>
#include <ae-core/project_editing/features/reverter.h>
#include <ae-core/project_editing/features/scroll_gesture_controller.h>
#include <ae-core/project_editing/features/time_editing_opener.h>
#include <ae-core/project_editing/features/timeline_updater.h>
#include <ae-core/project_editing/features/timing.h>
#include <ae-core/project_editing/features/waveform_mesh_importer.h>
#include <ae-core/project_editing/lifecycles/project_modal_lifecycle.h>
#include <ae-core/project_launching/features/project_launcher.h>

#include <ae-core/app/features/app_settings.hpp>
#include <ae-core/project/features/track_selector.hpp>
#include <ae-core/project/features/vertical_scrolling.hpp>
#include <ae-core/project_editing/features/deselector.hpp>
#include <ae-core/project_editing/features/display_space_range/display_space_range.hpp>
#include <ae-core/project_editing/features/display_space_time_range/display_space_time_range.hpp>
#include <ae-core/project_editing/features/display_space_track_range/display_space_track_range.hpp>
#include <ae-core/project_editing/features/escaper.hpp>
#include <ae-core/project_editing/features/grid_updater.hpp>
#include <ae-core/project_editing/features/marker_renaming_opener.hpp>
#include <ae-core/project_editing/features/marker_selector.hpp>
#include <ae-core/project_editing/features/module_selector.hpp>
#include <ae-core/project_editing/features/project_settings_opener.hpp>
#include <ae-core/project_editing/features/range_selector.hpp>
#include <ae-core/project_editing/features/selected_track_pool.hpp>
#include <ae-core/project_editing/features/selector_enabler.hpp>
#include <ae-core/project_editing/features/track_jumper.hpp>

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
      track_content_pool(track_content_pool::make_shared()),
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
      selected_track_pool(std::make_shared<ae::selected_track_pool>()),
      pasteboard(std::make_shared<ae::pasteboard>()),
      exporter(std::make_shared<ae::exporter>()),
      editing_status(std::make_shared<ae::editing_status>(this->exporter.get())),
      deselector(std::make_shared<ae::deselector>(this->selected_module_pool.get(), this->selected_marker_pool.get(),
                                                  this->selected_track_pool.get())),
      selector_enabler(std::make_shared<ae::selector_enabler>(
          this->selected_module_pool.get(), this->selected_marker_pool.get(), this->selected_track_pool.get())),
      track_selector(std::make_shared<ae::track_selector>(this->selected_track_pool.get(), this->editing_status.get(),
                                                          this->deselector.get(), this->selector_enabler.get())),
      module_selector(std::make_shared<ae::module_selector>(this->module_pool.get(), this->selected_module_pool.get(),
                                                            this->selected_track_pool.get(), this->editing_status.get(),
                                                            this->selector_enabler.get())),
      marker_selector(std::make_shared<ae::marker_selector>(this->marker_pool.get(), this->selected_marker_pool.get(),
                                                            this->editing_status.get(), this->deselector.get(),
                                                            this->selector_enabler.get())),
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
      marker_editor(std::make_shared<ae::marker_editor>(
          project_lifetime->player.get(), this->marker_pool.get(), this->database.get(), this->editing_status.get(),
          this->selected_marker_pool.get(), this->pasteboard.get(), this->selector_enabler.get())),
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
          project_lifetime->player.get(), this->module_pool.get(), this->selected_module_pool.get(),
          this->selected_track_pool.get(), project_lifetime->vertical_scrolling.get(), this->pasteboard.get(),
          this->editing_status.get(), this->selector_enabler.get())),
      range_selector(std::make_shared<ae::range_selector>(project_lifetime->player.get(), this->deselector.get())),
      display_space_time_range(std::make_shared<ae::display_space_time_range>(
          project_lifetime->project_format, project_lifetime->display_space.get(), project_lifetime->player.get())),
      display_space_track_range(std::make_shared<ae::display_space_track_range>(
          project_lifetime->display_space.get(), project_lifetime->vertical_scrolling.get())),
      display_space_range(std::make_shared<ae::display_space_range>(this->display_space_time_range.get(),
                                                                    this->display_space_track_range.get())),
      track_jumper(
          std::make_shared<ae::track_jumper>(this->module_pool.get(), project_lifetime->vertical_scrolling.get())),
      escaper(std::make_shared<ae::escaper>(this->pasteboard.get(), this->deselector.get())),
      receiver(std::make_shared<project_editing_receiver>(
          project_lifetime_id, this->database.get(), this->module_editor.get(), this->playing_toggler.get(),
          this->nudging.get(), this->nudger.get(), this->jumper.get(), this->edge_editor.get(),
          this->time_editing_opener.get(), this->marker_editor.get(), this->module_renaming_opener.get(),
          this->marker_renaming_opener.get(), this->settings_opener.get(), this->timing.get(),
          this->import_interactor.get(), this->export_interactor.get(), this->reverter.get(),
          this->module_selector.get(), this->marker_selector.get(), this->track_selector.get(), this->escaper.get(),
          this->pasteboard.get(), project_lifetime->vertical_scrolling.get(), this->track_jumper.get())) {
}
