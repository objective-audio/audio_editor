//
//  nudger.cpp
//

#include "nudger.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project_editing/features/nudging.h>
#include <cpp-utils/assertion.h>

using namespace yas;
using namespace yas::ae;

nudger::nudger(player *player, nudging *settings) : _player(player), _nudging(settings) {
}

bool nudger::can_nudge() const {
    return !this->_player->is_playing();
}

void nudger::nudge_previous(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();

    auto const prev_frame = this->_nudging->previous_nudging_frame(current_frame, offset_count);
    this->_player->seek(prev_frame);
}

void nudger::nudge_next(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();
    auto const next_frame = this->_nudging->next_nudging_frame(current_frame, offset_count);
    this->_player->seek(next_frame);
}
