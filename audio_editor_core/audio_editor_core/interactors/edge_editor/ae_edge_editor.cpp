//
//  ae_edge_editor.cpp
//

#include "ae_edge_editor.h"

#include <audio_editor_core/ae_edge_holder.h>
#include <audio_editor_core/ae_editing_status.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_editor> edge_editor::make_shared(project_id const &project_id, edge_holder *holder,
                                                      editing_status const *editing_status) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<edge_editor>(new edge_editor{holder, project_level->player.get(), editing_status});
}

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
