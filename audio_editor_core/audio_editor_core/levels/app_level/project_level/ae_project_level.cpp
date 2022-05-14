//
//  ae_project_level.cpp
//

#include "ae_project_level.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_context_menu_presenter.h>
#include <audio_editor_core/ae_dialog_presenter.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_info_loader.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_closer.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_launcher.h>
#include <audio_editor_core/ae_project_level_router.h>
#include <audio_editor_core/ae_project_state_holder.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_responder_stack.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_sheet_presenter.h>
#include <audio_editor_core/ae_system_url.h>
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
      editor_level_pool(project_editor_level_pool::make_shared(project_id)),
      state_holder(project_state_holder::make_shared()),
      closer(project_closer::make_shared(project_id, app_level->file_importer.get(),
                                         app_level->project_level_router.get(), this->editor_level_pool.get(),
                                         this->state_holder.get())),
      launcher(project_launcher::make_shared(project_id, file_url, app_level->file_info_loader.get(),
                                             this->responder_stack.get(), this->editor_level_pool.get(),
                                             this->state_holder.get())) {
}
