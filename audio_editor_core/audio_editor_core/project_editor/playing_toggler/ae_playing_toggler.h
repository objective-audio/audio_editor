//
//  ae_playing_toggler.h
//

#pragma once

#include <audio_editor_core/ae_playing_toggler_dependency.h>

#include <memory>

namespace yas::ae {
struct playing_toggler final {
    [[nodiscard]] static std::shared_ptr<playing_toggler> make_shared(
        std::shared_ptr<player_for_playing_toggler> const &);

    void toggle_playing();

   private:
    std::weak_ptr<player_for_playing_toggler> const _player;

    playing_toggler(std::shared_ptr<player_for_playing_toggler> const &);
};
}  // namespace yas::ae
