//
//  ae_project_action_sender.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_key.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class action_sender;

struct project_action_sender final {
    project_action_sender(window_lifetime_id const &, action_sender *);

    void send(action const &);
    void send(action_name const &, std::optional<action_value> &&value = std::nullopt);
    void send(ae::key const);

   private:
    window_lifetime_id const _window_lifetime_id;
    action_sender *const _action_sender;

    project_action_sender(project_action_sender const &) = delete;
    project_action_sender(project_action_sender &&) = delete;
    project_action_sender &operator=(project_action_sender const &) = delete;
    project_action_sender &operator=(project_action_sender &&) = delete;
};
}  // namespace yas::ae
