//
//  ae_marker_editor.cpp
//

#include "ae_marker_editor.h"

#include <audio_editor_core/ae_database.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_marker_pool.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

marker_editor::marker_editor(player const *player, marker_pool *marker_pool, database *database,
                             editing_status const *editing_status)
    : _player(player), _marker_pool(marker_pool), _database(database), _editing_status(editing_status) {
}

bool marker_editor::can_insert_marker() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const frame = this->_player->current_frame();
    return !this->_marker_pool->marker_for_frame(frame).has_value();
}

void marker_editor::insert_marker() {
    if (!this->can_insert_marker()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_marker_pool->insert_marker(current_frame);
}
