//
//  ae_playing_line_presenter.cpp
//

#include "ae_playing_line_presenter.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<playing_line_presenter> playing_line_presenter::make_shared(
    window_lifetime_id const &window_lifetime_id) {
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);
    return std::make_shared<playing_line_presenter>(window_lifetime->player);
}

playing_line_presenter::playing_line_presenter(std::shared_ptr<player> const &player) : _player(player) {
}

playing_line_state_t playing_line_presenter::playing_line_state() const {
    if (auto const player = this->_player.lock()) {
        if (player->is_scrolling()) {
            return playing_line_state_t::scrolling;
        } else if (player->is_playing()) {
            return playing_line_state_t::playing;
        }
    }

    return playing_line_state_t::pausing;
}
