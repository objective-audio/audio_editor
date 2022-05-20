//
//  ae_project_level.cpp
//

#include "ae_project_level.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_context_menu_presenter.h>
#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_database_updater.h>
#include <audio_editor_core/ae_dialog_presenter.h>
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
#include <audio_editor_core/ae_jumper.h>
#include <audio_editor_core/ae_marker_editor.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_module_renaming_launcher.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_nudger.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_playing_toggler.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_editor_launcher.h>
#include <audio_editor_core/ae_project_editor_responder.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_level_router.h>
#include <audio_editor_core/ae_project_state_holder.h>
#include <audio_editor_core/ae_project_sub_level_router.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_reverter.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_sheet_presenter.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_time_editor_launcher.h>
#include <audio_editor_core/ae_timeline_holder.h>
#include <audio_editor_core/ae_timeline_updater.h>
#include <audio_editor_core/ae_timing.h>
#include <audio_editor_core/ae_track_editor.h>
#include <audio_editor_core/ae_zooming_pair.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level> project_level::make_shared(ae::project_id const &project_id,
                                                          ae::project_format const &project_format,
                                                          url const &file_url) {
    return std::make_shared<project_level>(project_id, project_format, file_url, hierarchy::app_level());
}

project_level::project_level(ae::project_id const &project_id, ae::project_format const &project_format,
                             url const &file_url, std::shared_ptr<app_level> const &app_level)
    : project_id(project_id),
      project_format(project_format),
      file_url(file_url),
      project_url(project_url::make_shared(app_level->system_url->project_directory(project_id))),
      zooming_pair(zooming_pair::make_shared()),
      scrolling(scrolling::make_shared()),
      player(player::make_shared(app_level->system_url->playing_directory(), project_id, this->scrolling.get())),
      responder_stack(responder_stack::make_shared()),
      dialog_presenter(dialog_presenter::make_shared()),
      sheet_presenter(sheet_presenter::make_shared()),
      context_menu_presenter(context_menu_presenter::make_shared()),
      state_holder(project_state_holder::make_shared()),
      closer(project_closer::make_shared(project_id, app_level->file_importer.get(),
                                         app_level->project_level_router.get(), this->state_holder.get())),
      timing(timing::make_shared(project_format.sample_rate)),
      nudge_settings(nudge_settings::make_shared(this->timing.get())),
      file_track(file_track::make_shared()),
      marker_pool(marker_pool::make_shared()),
      pasteboard(pasteboard::make_shared()),
      database(database::make_shared(this->project_url->db_file())),
      exporter(exporter::make_shared()),
      editing_status(editing_status::make_shared(this->exporter.get())),
      playing_toggler(playing_toggler::make_shared(this->player.get())),
      sub_level_router(project_sub_level_router::make_shared(project_id)),
      timeline_holder(timeline_holder::make_shared(this->project_url->editing_file(), this->project_format)),
      nudger(nudger::make_shared(this->player.get(), this->nudge_settings.get())),
      edge_holder(edge_holder::make_shared()),
      edge_editor(edge_editor::make_shared(this->edge_holder.get(), this->player.get(), this->editing_status.get())),
      jumper(jumper::make_shared(this->player.get(), this->file_track.get(), this->marker_pool.get(),
                                 this->edge_holder.get())),
      time_editor_launcher(time_editor_launcher::make_shared(
          this->player.get(), this->timing.get(), this->sub_level_router.get(), this->responder_stack.get())),
      marker_editor(marker_editor::make_shared(this->player.get(), this->marker_pool.get(), this->database.get(),
                                               this->editing_status.get())),
      module_renaming_launcher(
          module_renaming_launcher::make_shared(this->sub_level_router.get(), this->editing_status.get())),
      export_interactor(export_interactor::make_shared(
          project_format, this->dialog_presenter.get(), this->editing_status.get(), this->edge_holder.get(),
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
      track_editor(track_editor::make_shared(this->player.get(), this->file_track.get(), this->marker_pool.get(),
                                             this->pasteboard.get(), this->database.get(), this->editing_status.get())),
      editor_launcher(project_editor_launcher::make_shared(file_url, this->file_module_loader.get())),
      responder(project_editor_responder::make_shared(
          this->track_editor.get(), this->playing_toggler.get(), this->nudge_settings.get(), this->nudger.get(),
          this->jumper.get(), this->edge_editor.get(), this->time_editor_launcher.get(), this->marker_editor.get(),
          this->module_renaming_launcher.get(), this->timing.get(), this->export_interactor.get(),
          this->reverter.get())),
      launcher(project_launcher::make_shared(this->instance_id, file_url, app_level->file_info_loader.get(),
                                             this->responder_stack.get(), this->state_holder.get(), this->responder)) {
    this->editor_launcher->launch();
}
