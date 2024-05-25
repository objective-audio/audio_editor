//
//  playing_toggler.cpp
//

#include "playing_toggler.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/player.h>

using namespace yas;
using namespace yas::ae;

playing_toggler::playing_toggler(player_for_playing_toggler *player) : _player(player) {
}

void playing_toggler::toggle_playing() {
    this->_player->set_playing(!this->_player->is_playing());
}
