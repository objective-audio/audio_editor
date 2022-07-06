//
//  ae_editing_root_presenter.cpp
//

#include "ae_editing_root_presenter.h"

#include <audio_editor_core/ae_action_sender.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_responding.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<editing_root_presenter> editing_root_presenter::make_shared(project_id const &project_id) {
    auto const &app_lifetime = hierarchy::app_lifetime();
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_id);
    return std::make_shared<editing_root_presenter>(project_id, project_lifetime->player, app_lifetime->action_sender);
}

editing_root_presenter::editing_root_presenter(project_id const &project_id, std::shared_ptr<player> const &player,
                                               std::shared_ptr<action_sender> const &action_sender)
    : _project_id(project_id), _player(player), _action_sender(action_sender) {
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
