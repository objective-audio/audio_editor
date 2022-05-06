//
//  ae_playing_toggler.h
//

#pragma once

#include <audio_editor_core/ae_playing_toggler_dependency.h>

#include <memory>

namespace yas::ae {
struct playing_toggler final {
    [[nodiscard]] static std::shared_ptr<playing_toggler> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<playing_toggler> make_shared(player_for_playing_toggler *);

    void toggle_playing();

   private:
    player_for_playing_toggler *const _player;

    playing_toggler(player_for_playing_toggler *);
};
}  // namespace yas::ae
