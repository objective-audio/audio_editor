//
//  ae_playing_toggler.cpp
//

#include "ae_playing_toggler.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<playing_toggler> playing_toggler::make_shared(player_for_playing_toggler *player) {
    return std::make_shared<playing_toggler>(player);
}

playing_toggler::playing_toggler(player_for_playing_toggler *player) : _player(player) {
}

void playing_toggler::toggle_playing() {
    this->_player->set_playing(!this->_player->is_playing());
}
