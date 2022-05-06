//
//  ae_nudger.cpp
//

#include "ae_nudger.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_player.h>
#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<nudger> nudger::make_shared(std::string const &project_id, nudge_settings *settings) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_level->player.get(), settings);
}

std::shared_ptr<nudger> nudger::make_shared(player *player, nudge_settings *settings) {
    return std::shared_ptr<nudger>(new nudger{player, settings});
}

nudger::nudger(player *player, nudge_settings *settings) : _player(player), _settings(settings) {
}

bool nudger::can_nudge() const {
    return !this->_player->is_playing();
}

void nudger::nudge_previous(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();

    if (auto const prev_frame = this->_settings->previous_frame(current_frame, offset_count)) {
        this->_player->seek(prev_frame.value());
    }
}

void nudger::nudge_next(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    frame_index_t const current_frame = this->_player->current_frame();
    if (auto const next_frame = this->_settings->next_frame(current_frame, offset_count)) {
        this->_player->seek(next_frame.value());
    }
}
