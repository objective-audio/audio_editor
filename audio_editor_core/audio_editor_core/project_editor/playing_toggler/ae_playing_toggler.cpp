//
//  ae_playing_toggler.cpp
//

#include "ae_playing_toggler.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<playing_toggler> playing_toggler::make_shared(std::string const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_level->player);
}

std::shared_ptr<playing_toggler> playing_toggler::make_shared(
    std::shared_ptr<player_for_playing_toggler> const &player) {
    return std::shared_ptr<playing_toggler>(new playing_toggler{player});
}

playing_toggler::playing_toggler(std::shared_ptr<player_for_playing_toggler> const &player) : _player(player) {
}

void playing_toggler::toggle_playing() {
    auto const player = this->_player.lock();
    if (!player) {
        return;
    }

    player->set_playing(!player->is_playing());
}
