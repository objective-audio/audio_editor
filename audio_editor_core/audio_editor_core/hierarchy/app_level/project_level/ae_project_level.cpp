//
//  ae_project_level.cpp
//

#include "ae_project_level.h"

#include <audio_editor_core/ae_action_router.h>
#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_context_menu_presenter.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor_level_maker.h>
#include <audio_editor_core/ae_project_editor_level_pool.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level> project_level::make_shared(std::string const &identifier, url const &file_url) {
    return std::shared_ptr<project_level>(new project_level{identifier, file_url, app_level::global()});
}

project_level::project_level(std::string const &identifier, url const &file_url,
                             std::shared_ptr<app_level> const &app_level)
    : identifier(identifier),
      file_url(file_url),
      project_url(project_url::make_shared(app_level->system_url->project_directory(identifier))),
      horizontal_zooming(zooming::make_shared()),
      vertical_zooming(zooming::make_shared()),
      scrolling(scrolling::make_shared()),
      player(player::make_shared(app_level->system_url->playing_directory(), identifier, this->scrolling)),
      action_router(action_router::make_shared()),
      action_controller(action_controller::make_shared(this->action_router)),
      dialog_presenter(dialog_presenter::make_shared()),
      context_menu_presenter(context_menu_presenter::make_shared()),
      editor_maker(project_editor_level_maker::make_shared(identifier, this->player, this->action_controller,
                                                           this->dialog_presenter)),
      editor_level_pool(project_editor_level_pool::make_shared(this->editor_maker)),
      project(project::make_shared(identifier, file_url, this->project_url, app_level->file_importer,
                                   app_level->file_loader, this->editor_level_pool)) {
    this->project->setup();
}
