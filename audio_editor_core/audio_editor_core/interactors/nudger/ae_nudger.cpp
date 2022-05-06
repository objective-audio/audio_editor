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

std::shared_ptr<nudger> nudger::make_shared(std::string const &project_id,
                                            std::shared_ptr<nudge_settings> const &settings) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return make_shared(project_level->player, settings);
}

std::shared_ptr<nudger> nudger::make_shared(std::shared_ptr<player> const &player,
                                            std::shared_ptr<nudge_settings> const &settings) {
    return std::shared_ptr<nudger>(new nudger{player, settings});
}

nudger::nudger(std::shared_ptr<player> const &player, std::shared_ptr<nudge_settings> const &settings)
    : _player(player), _settings(settings) {
}

bool nudger::can_nudge() const {
    auto const player = this->_player.lock();
    if (!player) {
        assertion_failure_if_not_test();
        return false;
    }

    return !player->is_playing();
}

void nudger::nudge_previous(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    auto const player = this->_player.lock();
    auto const settings = this->_settings.lock();

    if (!player || !settings) {
        assertion_failure_if_not_test();
        return;
    }

    frame_index_t const current_frame = player->current_frame();

    if (auto const prev_frame = settings->previous_frame(current_frame, offset_count)) {
        player->seek(prev_frame.value());
    }
}

void nudger::nudge_next(uint32_t const offset_count) {
    if (!this->can_nudge()) {
        return;
    }

    auto const player = this->_player.lock();
    auto const settings = this->_settings.lock();

    if (!player || !settings) {
        assertion_failure_if_not_test();
        return;
    }

    frame_index_t const current_frame = player->current_frame();
    if (auto const next_frame = settings->next_frame(current_frame, offset_count)) {
        player->seek(next_frame.value());
    }
}
