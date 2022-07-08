//
//  ae_editing_root_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class player;
class action_sender;

struct editing_root_presenter final {
    [[nodiscard]] static std::shared_ptr<editing_root_presenter> make_shared(window_lifetime_id const &);

    editing_root_presenter(std::shared_ptr<player> const &, std::shared_ptr<action_sender> const &);

    [[nodiscard]] playing_line_state_t playing_line_state() const;

    [[nodiscard]] bool responds_to_action(action const);

   private:
    std::weak_ptr<player> const _player;
    std::weak_ptr<action_sender> const _action_sender;

    editing_root_presenter(editing_root_presenter const &) = delete;
    editing_root_presenter(editing_root_presenter &&) = delete;
    editing_root_presenter &operator=(editing_root_presenter const &) = delete;
    editing_root_presenter &operator=(editing_root_presenter &&) = delete;
};
}  // namespace yas::ae
