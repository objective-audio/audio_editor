//
//  ae_project_level.cpp
//

#include "ae_project_level.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor_maker.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_level> project_level::make_shared(std::string const &identifier, url const &file_url) {
    auto const app_level = app_level::global();
    auto const project_url = project_url::make_shared(app_level->system_url->project_directory(identifier));
    auto const file_importer = app_level->file_importer;
    auto const file_loader = app_level->file_loader;
    auto const scrolling = scrolling::make_shared();
    auto const player = player::make_shared(app_level->system_url->playing_directory(), identifier, scrolling);
    auto const action_router = action_router::make_shared();
    auto const action_controller = action_controller::make_shared(action_router);
    auto const dialog_presenter = dialog_presenter::make_shared();
    auto const context_menu_presenter = context_menu_presenter::make_shared();
    auto const editor_maker = project_editor_maker::make_shared(player, action_controller, dialog_presenter);
    auto const project =
        project::make_shared(identifier, file_url, project_url, file_importer, file_loader, player, editor_maker,
                             action_controller, dialog_presenter, context_menu_presenter, action_router);

    return std::shared_ptr<project_level>(new project_level{identifier, scrolling, project});
}

project_level::project_level(std::string const &identifier, std::shared_ptr<ae::scrolling> const &scrolling,
                             std::shared_ptr<ae::project> const &project)
    : identifier(identifier),
      horizontal_zooming(zooming::make_shared()),
      vertical_zooming(zooming::make_shared()),
      scrolling(scrolling),
      project(project) {
}
