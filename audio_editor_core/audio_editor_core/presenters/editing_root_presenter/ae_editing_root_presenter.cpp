//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_responder_stack.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(project_id const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::make_shared<editing_root_presenter>(project_level->player, project_level->responder_stack);
}

editing_root_presenter::editing_root_presenter(std::shared_ptr<player> const &player,
                                               std::shared_ptr<responder_stack> const &responder_stack)
    : _player(player), _responder_stack(responder_stack) {
}

playing_line_state_t editing_root_presenter::playing_line_state() const {
    if (auto const player = this->_player.lock()) {
        if (player->is_scrolling()) {
            return playing_line_state_t::scrolling;
        } else if (player->is_playing()) {
            return playing_line_state_t::playing;
        }
    }

    return playing_line_state_t::pausing;
}

bool editing_root_presenter::responds_to_action(action const action) {
    if (auto const responder_stack = this->_responder_stack.lock()) {
        auto const responding = responder_stack->responding_to_action(action);
        switch (responding) {
            case responding::accepting:
            case responding::blocking:
                return true;

            case responding::fallthrough:
                return false;
        }
    } else {
        return false;
    }
}
