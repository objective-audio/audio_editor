//
//  ae_project_editor_level_pool.cpp
//

#include "ae_project_editor_level_pool.h"

#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<project_editor_level_pool> project_editor_level_pool::make_shared(
    std::string const &identifier, std::shared_ptr<player_for_project_editor> const &player,
    std::shared_ptr<action_controller> const &action_controller,
    std::shared_ptr<dialog_presenter> const &dialog_presenter) {
    return std::shared_ptr<project_editor_level_pool>(
        new project_editor_level_pool{identifier, player, action_controller, dialog_presenter});
}

project_editor_level_pool::project_editor_level_pool(std::string const &identifier,
                                                     std::shared_ptr<player_for_project_editor> const &player,
                                                     std::shared_ptr<action_controller> const &action_controller,
                                                     std::shared_ptr<dialog_presenter> const &dialog_presenter)
    : _identifier(identifier),
      _player(player),
      _action_controller(action_controller),
      _dialog_presenter(dialog_presenter),
      _editor_level(nullptr) {
}

void project_editor_level_pool::add_editor_level(file_info const &file_info) {
    if (this->_editor_level != nullptr) {
        throw std::runtime_error("editor_level is not null.");
    }

    auto const timing = timing::make_shared(file_info.sample_rate);
    auto const nudging = nudging::make_shared(timing);
    auto const editor = project_editor::make_shared(this->_identifier, file_info, this->_player,
                                                    this->_action_controller, this->_dialog_presenter, nudging, timing);

    this->_editor_level = project_editor_level::make_shared(editor);
}

std::shared_ptr<project_editor_level> const &project_editor_level_pool::editor_level() const {
    return this->_editor_level;
}
