//
//  ae_project_editor_maker.cpp
//

#include "ae_project_editor_maker.h"

#include <audio_editor_core/ae_project_editor.h>

using namespace yas;
using namespace yas::ae;

project_editor_maker::project_editor_maker(std::shared_ptr<player_for_project_editor> const &player,
                                           std::shared_ptr<action_controller> const &action_controller)
    : _player(player), _action_controller(action_controller) {
}

std::shared_ptr<project_editor_for_project> project_editor_maker::make(url const &editing_file_url,
                                                                       url const &db_file_url,
                                                                       file_info const &file_info) const {
    return project_editor::make_shared(editing_file_url, db_file_url, file_info, this->_player,
                                       this->_action_controller);
}

std::shared_ptr<project_editor_maker> project_editor_maker::make_shared(
    std::shared_ptr<player_for_project_editor> const &player,
    std::shared_ptr<action_controller> const &action_controller) {
    return std::shared_ptr<project_editor_maker>(new project_editor_maker{player, action_controller});
}
