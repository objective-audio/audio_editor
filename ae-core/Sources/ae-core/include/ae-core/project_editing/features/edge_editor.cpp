//
//  edge_editor.cpp
//

#include "edge_editor.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project_editing/features/database.h>
#include <ae-core/project_editing/features/edge_holder.h>
#include <ae-core/project_editing/features/editing_status.h>
#include <cpp-utils/assertion.h>

using namespace yas;
using namespace yas::ae;

edge_editor::edge_editor(edge_holder *holder, player const *player, editing_status const *editing_status)
    : _holder(holder), _player(player), _editing_status(editing_status) {
}

bool edge_editor::can_set_begin() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    auto const begin_frame = this->_holder->edge().begin_frame;

    return current_frame != begin_frame;
}

bool edge_editor::can_set_end() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    auto const current_frame = this->_player->current_frame();
    auto const end_frame = this->_holder->edge().end_frame;

    return current_frame != end_frame;
}

void edge_editor::set_begin() {
    if (!this->can_set_begin()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_holder->set_begin_frame(current_frame);
}

void edge_editor::set_end() {
    if (!this->can_set_end()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    this->_holder->set_end_frame(current_frame);
}
