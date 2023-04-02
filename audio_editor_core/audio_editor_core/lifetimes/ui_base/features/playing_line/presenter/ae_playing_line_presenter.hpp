//
//  ae_playing_line_presenter.hpp
//

#pragma once

#include <audio_editor_core/ae_playing_line_state.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class player;

struct playing_line_presenter final {
    [[nodiscard]] static std::shared_ptr<playing_line_presenter> make_shared(window_lifetime_id const &);

    playing_line_presenter(std::shared_ptr<player> const &);

    [[nodiscard]] playing_line_state_t playing_line_state() const;

   private:
    std::weak_ptr<player> const _player;

    playing_line_presenter(playing_line_presenter const &) = delete;
    playing_line_presenter(playing_line_presenter &&) = delete;
    playing_line_presenter &operator=(playing_line_presenter const &) = delete;
    playing_line_presenter &operator=(playing_line_presenter &&) = delete;
};
}  // namespace yas::ae
