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
#include <audio_editor_core/ae_module_renaming_opener.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_project_action_sender.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_project_receiver.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_reverter.h>
#include <audio_editor_core/ae_scroll_gesture_controller.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_time_editor_opener.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <audio_editor_core/ae_timing.h>
#include <audio_editor_core/ae_track_editor.h>
#include <audio_editor_core/ae_waveform_mesh_importer.h>
#include <audio_editor_core/ae_window_lifecycle.h>
#include <audio_editor_core/ae_zooming_pair.h>

#include <audio_editor_core/ae_marker_renaming_opener.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_lifetime> project_lifetime::make_shared(ae::window_lifetime_id const &window_lifetime_id) {
    auto const window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);

    return std::make_shared<project_lifetime>(window_lifetime.get(), hierarchy::app_lifetime().get());
}

project_lifetime::project_lifetime(window_lifetime *window_lifetime, app_lifetime *app_lifetime)
    : window_lifetime_id(window_lifetime->lifetime_id),
      project_format(window_lifetime->project_format),
      module_location_pool(module_location_pool::make_shared()),
      marker_location_pool(marker_location_pool::make_shared()),
      action_sender(ae::project_action_sender::make_shared(window_lifetime_id, app_lifetime->action_sender.get())),
      pinch_gesture_controller(ae::pinch_gesture_controller::make_shared(window_lifetime->zooming_pair.get())),
      scroll_gesture_controller(std::make_shared<ae::scroll_gesture_controller>(window_lifetime->scrolling.get())),
      timing(timing::make_shared(project_format.sample_rate)),
      nudge_settings(nudge_settings::make_shared(this->timing.get())),
      file_track(file_track::make_shared()),
      waveforms_mesh_importer(
          waveform_mesh_importer::make_shared(window_lifetime->lifetime_id, this->file_track.get())),
      marker_pool(marker_pool::make_shared()),
      pasteboard(pasteboard::make_shared()),
      database(database::make_shared(window_lifetime->project_url->db_file())),
      exporter(exporter::make_shared()),
      editing_status(editing_status::make_shared(this->exporter.get())),
      playing_toggler(playing_toggler::make_shared(window_lifetime->player.get())),
      modal_lifecycle(project_modal_lifecycle::make_shared(window_lifetime_id)),
      nudger(nudger::make_shared(window_lifetime->player.get(), this->nudge_settings.get())),
      edge_holder(edge_holder::make_shared()),
      edge_editor(std::make_shared<ae::edge_editor>(this->edge_holder.get(), window_lifetime->player.get(),
                                                    this->editing_status.get(), this->database.get())),
      jumper(jumper::make_shared(window_lifetime->player.get(), this->file_track.get(), this->marker_pool.get(),
                                 this->edge_holder.get())),
      time_editor_opener(time_editor_opener::make_shared(window_lifetime->player.get(), this->timing.get(),
                                                         this->modal_lifecycle.get())),
      marker_editor(marker_editor::make_shared(window_lifetime->player.get(), this->marker_pool.get(),
                                               this->database.get(), this->editing_status.get())),
      module_renaming_opener(
          module_renaming_opener::make_shared(this->modal_lifecycle.get(), this->editing_status.get())),
      marker_renaming_opener(
          marker_renaming_opener::make_shared(this->modal_lifecycle.get(), this->editing_status.get())),
      export_interactor(export_interactor::make_shared(
          project_format, this->modal_lifecycle.get(), this->editing_status.get(), this->edge_holder.get(),
          window_lifetime->player.get(), this->exporter.get(), window_lifetime->timeline_holder.get())),
      database_updater(database_updater::make_shared(this->file_track.get(), this->marker_pool.get(),
                                                     this->edge_holder.get(), this->pasteboard.get(),
                                                     this->database.get())),
      timeline_updater(timeline_updater::make_shared(this->file_track.get(), window_lifetime->timeline_holder.get())),
      reverter(reverter::make_shared(this->database.get(), this->file_track.get(), this->marker_pool.get(),
                                     this->pasteboard.get(), this->edge_holder.get(), this->editing_status.get())),
      file_module_loading_state_holder(file_module_loading_state_holder::make_shared()),
      file_module_loader(file_module_loader::make_shared(
          window_lifetime_id.project, window_lifetime->project_url.get(), this->project_format,
          window_lifetime->player.get(), this->file_module_loading_state_holder.get(), this->database.get(),
          this->file_track.get(), this->edge_holder.get(), window_lifetime->timeline_holder.get())),
      import_interactor(import_interactor::make_shared(this->modal_lifecycle.get(), this->editing_status.get(),
                                                       this->file_module_loader.get())),
      track_editor(track_editor::make_shared(window_lifetime->player.get(), this->file_track.get(),
                                             this->marker_pool.get(), this->pasteboard.get(), this->database.get(),
                                             this->editing_status.get())),
      receiver(std::make_shared<project_receiver>(
          window_lifetime_id, this->track_editor.get(), this->playing_toggler.get(), this->nudge_settings.get(),
          this->nudger.get(), this->jumper.get(), this->edge_editor.get(), this->time_editor_opener.get(),
          this->marker_editor.get(), this->module_renaming_opener.get(), this->marker_renaming_opener.get(),
          this->timing.get(), this->import_interactor.get(), this->export_interactor.get(), this->reverter.get())) {
}
