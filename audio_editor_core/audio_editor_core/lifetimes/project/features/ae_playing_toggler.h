//
//  ae_playing_toggler.h
//

#pragma once

#include <audio_editor_core/ae_playing_toggler_dependency.h>

#include <memory>

namespace yas::ae {
struct playing_toggler final {
    explicit playing_toggler(player_for_playing_toggler *);

    void toggle_playing();

   private:
    player_for_playing_toggler *const _player;

    playing_toggler(playing_toggler const &) = delete;
    playing_toggler(playing_toggler &&) = delete;
    playing_toggler &operator=(playing_toggler const &) = delete;
    playing_toggler &operator=(playing_toggler &&) = delete;
};
}  // namespace yas::ae
