//
//  ae_project_maker.cpp
//

#include "ae_project_maker.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_file_importer.h>
#include <audio_editor_core/ae_file_loader.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor_maker.h>
#include <audio_editor_core/ae_project_url.h>
#include <audio_editor_core/ae_scrolling.h>
#include <audio_editor_core/ae_system_url.h>
#include <audio_editor_core/ae_uuid_generator.h>
#include <audio_editor_core/ae_zooming.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_maker> project_maker::make_shared() {
    return make_shared(uuid_generator::make_shared());
}

std::shared_ptr<project_maker> project_maker::make_shared(
    std::shared_ptr<uuid_generator_for_project_maker> const &uuid_generator) {
    return std::shared_ptr<project_maker>(new project_maker{uuid_generator});
}

project_maker::project_maker(std::shared_ptr<uuid_generator_for_project_maker> const &uuid_generator)
    : _uuid_generator(uuid_generator) {
}

std::shared_ptr<project_for_project_pool> project_maker::make(url const &file_url) {
    auto const identifier = this->_uuid_generator->generate();
    auto const app_level = app_level::global();
    auto const project_url = project_url::make_shared(app_level->system_url->project_directory(identifier));
    auto const file_importer = app_level->file_importer;
    auto const file_loader = app_level->file_loader;
    auto const scrolling = scrolling::make_shared();
    auto const horizontal_zooming = zooming::make_shared();
    auto const vertical_zooming = zooming::make_shared();
    auto const player = player::make_shared(app_level->system_url->playing_directory(), identifier, scrolling);
    auto const action_router = action_router::make_shared();
    auto const action_controller = action_controller::make_shared(action_router);
    auto const dialog_presenter = dialog_presenter::make_shared();
    auto const context_menu_presenter = context_menu_presenter::make_shared();
    auto const editor_maker = project_editor_maker::make_shared(player, action_controller, dialog_presenter);
    return project::make_shared(identifier, file_url, project_url, file_importer, file_loader, player, editor_maker,
                                horizontal_zooming, vertical_zooming, scrolling, action_controller, dialog_presenter,
                                context_menu_presenter, action_router);
}
