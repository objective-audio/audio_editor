//
//  ae_timeline_editor.cpp
//

#include "ae_timeline_editor.h"

#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

timeline_editor::timeline_editor(std::shared_ptr<timeline_player_interface> const &player)
    : _timeline(proc::timeline::make_shared()), _player(player) {
    this->_player->set_timeline(this->_timeline);
}

std::shared_ptr<timeline_editor> timeline_editor::make_shared(
    std::shared_ptr<timeline_player_interface> const &player) {
    return std::shared_ptr<timeline_editor>(new timeline_editor{player});
}
