//
//  ae_project_lifetime.cpp
//

#include "ae_project_lifetime.h"

#include <audio_editor_core/ae_app_lifetime.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_database_updater.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_export_interactor.h>
#include <audio_editor_core/ae_exporter.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_file_module_loader.h>
#include <audio_editor_core/ae_file_module_loading_state_holder.h>
#include <audio_editor_core/ae_file_track.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_import_interactor.h>
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_editor.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_module_renaming_launcher.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_project_action_controller.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_editor_responder.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_project_receiver.h>
#include <audio_editor_core/ae_project_state_holder.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_reverter.h>
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_time_editor_launcher.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <audio_editor_core/ae_timing.h>
#include <audio_editor_core/ae_track_editor.h>
#include <audio_editor_core/ae_waveform_mesh_importer.h>
#include <audio_editor_core/ae_window_lifecycle.h>
#include <audio_editor_core/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_lifetime> project_lifetime::make_shared(ae::project_id const &project_id) {
    auto const window_lifetime = hierarchy::window_lifetime_for_id(project_id);

    return make_shared(project_id, window_lifetime->project_format, window_lifetime->project_directory_url);
}

std::shared_ptr<project_lifetime> project_lifetime::make_shared(ae::project_id const &project_id,
                                                                ae::project_format const &project_format,
                                                                url const &project_dir_url) {
    return std::make_shared<project_lifetime>(project_id, project_format, project_dir_url, hierarchy::app_lifetime());
}

project_lifetime::project_lifetime(ae::project_id const &project_id, ae::project_format const &project_format,
                                   url const &project_dir_url, std::shared_ptr<app_lifetime> const &app_lifetime)
    : project_id(project_id),
      project_format(project_format),
      project_url(project_url::make_shared(project_dir_url)),
      zooming_pair(zooming_pair::make_shared()),
      scrolling(scrolling::make_shared()),
      player(player::make_shared(app_lifetime->system_url->playing_directory(), project_id, this->scrolling.get())),
      state_holder(project_state_holder::make_shared()),
      closer(project_closer::make_shared(project_id, app_lifetime->file_importer.get(),
                                         app_lifetime->window_lifecycle.get(), this->state_holder.get())),
      module_location_pool(module_location_pool::make_shared()),
      marker_location_pool(marker_location_pool::make_shared()),
      waveforms_mesh_importer(waveform_mesh_importer::make_shared(this->project_url.get())),
      action_controller(ae::project_action_controller::make_shared(project_id, app_lifetime->action_sender.get())),
      pinch_gesture_controller(ae::pinch_gesture_controller::make_shared(this->zooming_pair.get())),
      scroll_gesture_controller(std::make_shared<ae::scroll_gesture_controller>(this->scrolling.get())),
      timing(timing::make_shared(project_format.sample_rate)),
      nudge_settings(nudge_settings::make_shared(this->timing.get())),
      file_track(file_track::make_shared()),
      marker_pool(marker_pool::make_shared()),
      pasteboard(pasteboard::make_shared()),
      database(database::make_shared(this->project_url->db_file())),
      exporter(exporter::make_shared()),
      editing_status(editing_status::make_shared(this->exporter.get())),
      playing_toggler(playing_toggler::make_shared(this->player.get())),
      modal_lifecycle(project_modal_lifecycle::make_shared(project_id)),
      timeline_holder(timeline_holder::make_shared(this->project_format, this->project_url.get())),
      nudger(nudger::make_shared(this->player.get(), this->nudge_settings.get())),
      edge_holder(edge_holder::make_shared()),
      edge_editor(edge_editor::make_shared(this->edge_holder.get(), this->player.get(), this->editing_status.get())),
      jumper(jumper::make_shared(this->player.get(), this->file_track.get(), this->marker_pool.get(),
                                 this->edge_holder.get())),
      time_editor_launcher(
          time_editor_launcher::make_shared(this->player.get(), this->timing.get(), this->modal_lifecycle.get())),
      marker_editor(marker_editor::make_shared(this->player.get(), this->marker_pool.get(), this->database.get(),
                                               this->editing_status.get())),
      module_renaming_launcher(
          module_renaming_launcher::make_shared(this->modal_lifecycle.get(), this->editing_status.get())),
      export_interactor(export_interactor::make_shared(
          project_format, this->modal_lifecycle.get(), this->editing_status.get(), this->edge_holder.get(),
          this->player.get(), this->exporter.get(), this->timeline_holder.get())),
      database_updater(database_updater::make_shared(this->file_track.get(), this->marker_pool.get(),
                                                     this->edge_holder.get(), this->pasteboard.get(),
                                                     this->database.get())),
      timeline_updater(timeline_updater::make_shared(this->file_track.get(), this->timeline_holder.get())),
      reverter(reverter::make_shared(this->database.get(), this->file_track.get(), this->marker_pool.get(),
                                     this->pasteboard.get(), this->edge_holder.get(), this->editing_status.get())),
      file_module_loading_state_holder(file_module_loading_state_holder::make_shared()),
      file_module_loader(file_module_loader::make_shared(
          project_id, this->project_url.get(), this->project_format, this->player.get(),
          this->file_module_loading_state_holder.get(), this->database.get(), this->file_track.get(),
          this->edge_holder.get(), this->timeline_holder.get())),
      import_interactor(import_interactor::make_shared(this->modal_lifecycle.get(), this->editing_status.get(),
                                                       this->file_module_loader.get())),
      track_editor(track_editor::make_shared(this->player.get(), this->file_track.get(), this->marker_pool.get(),
                                             this->pasteboard.get(), this->database.get(), this->editing_status.get())),
      responder(project_editor_responder::make_shared(
          project_id, this->track_editor.get(), this->playing_toggler.get(), this->nudge_settings.get(),
          this->nudger.get(), this->jumper.get(), this->edge_editor.get(), this->time_editor_launcher.get(),
          this->marker_editor.get(), this->module_renaming_launcher.get(), this->timing.get(),
          this->import_interactor.get(), this->export_interactor.get(), this->reverter.get())),
      receiver(project_receiver::make_shared(project_id, this->responder.get())),
      launcher(project_launcher::make_shared(this->instance_id, this->project_format, this->state_holder.get(),
                                             this->player.get(), this->timeline_holder.get(), this->responder)) {
}
