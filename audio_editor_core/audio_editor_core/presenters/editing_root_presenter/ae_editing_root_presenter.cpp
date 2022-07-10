//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_action_receivable_state.h>
#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(
    window_lifetime_id const &window_lifetime_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &window_lifetime = hierarchy::window_lifetime_for_id(window_lifetime_id);
    return std::make_shared<editing_root_presenter>(window_lifetime->player, app_lifetime->action_sender);
}

editing_root_presenter::editing_root_presenter(std::shared_ptr<player> const &player,
                                               std::shared_ptr<action_sender> const &action_sender)
    : _player(player), _action_sender(action_sender) {
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
    if (auto const action_sender = this->_action_sender.lock()) {
        switch (action_sender->responding_to_action(action)) {
            case action_receivable_state::accepting:
            case action_receivable_state::blocking:
                return true;

            case action_receivable_state::fallthrough:
                return false;
        }
    } else {
        return false;
    }
}
