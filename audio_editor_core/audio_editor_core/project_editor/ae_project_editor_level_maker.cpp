//
//  ae_project_editor_maker.cpp
//

#include "ae_project_editor_level_maker.h"

#include <audio_editor_core/ae_nudging.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_editor_level.h>
#include <audio_editor_core/ae_timing.h>

using namespace yas;
using namespace yas::ae;

project_editor_level_maker::project_editor_level_maker(std::string const &identifier,
                                                       std::shared_ptr<player_for_project_editor> const &player,
                                                       std::shared_ptr<action_controller> const &action_controller,
                                                       std::shared_ptr<dialog_presenter> const &dialog_presenter)
    : _identifier(identifier),
      _player(player),
      _action_controller(action_controller),
      _dialog_presenter(dialog_presenter) {
}

std::shared_ptr<project_editor_level_maker> project_editor_level_maker::make_shared(
    std::string const &identifier, std::shared_ptr<player_for_project_editor> const &player,
    std::shared_ptr<action_controller> const &action_controller,
    std::shared_ptr<dialog_presenter> const &dialog_presenter) {
    return std::shared_ptr<project_editor_level_maker>(
        new project_editor_level_maker{identifier, player, action_controller, dialog_presenter});
}

std::shared_ptr<project_editor_level> project_editor_level_maker::make(file_info const &file_info) const {
    auto const timing = timing::make_shared(file_info.sample_rate);
    auto const nudging = nudging::make_shared(timing);
    auto const editor = project_editor::make_shared(this->_identifier, file_info, this->_player,
                                                    this->_action_controller, this->_dialog_presenter, nudging, timing);
    return project_editor_level::make_shared(editor);
}
