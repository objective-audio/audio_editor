//
//  ae_playing_toggler.cpp
//

#include "ae_playing_toggler.h"

using namespace yas;
using namespace yas::ae;

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
