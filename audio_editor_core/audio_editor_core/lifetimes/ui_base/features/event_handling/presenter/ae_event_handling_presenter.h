//
//  ae_event_handling_presenter.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class player;
class action_sender;

struct event_handling_presenter final {
    [[nodiscard]] static std::shared_ptr<event_handling_presenter> make_shared(window_lifetime_id const &);

    event_handling_presenter(window_lifetime_id const &, std::shared_ptr<action_sender> const &);

    [[nodiscard]] bool responds_to_action(action const);

   private:
    window_lifetime_id const _window_lifetime_id;
    std::weak_ptr<action_sender> const _action_sender;

    event_handling_presenter(event_handling_presenter const &) = delete;
    event_handling_presenter(event_handling_presenter &&) = delete;
    event_handling_presenter &operator=(event_handling_presenter const &) = delete;
    event_handling_presenter &operator=(event_handling_presenter &&) = delete;
};
}  // namespace yas::ae
