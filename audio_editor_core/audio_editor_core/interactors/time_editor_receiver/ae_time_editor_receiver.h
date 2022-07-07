//
//  ae_time_editor_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
class time_editor;
class time_editor_closer;
class time_editor_responder;

struct time_editor_receiver : action_receivable {
    [[nodiscard]] static std::shared_ptr<time_editor_receiver> make_shared(window_lifetime_id const &, time_editor *,
                                                                           time_editor_closer *);

    time_editor_receiver(window_lifetime_id const &, time_editor *, time_editor_closer *);

   private:
    window_lifetime_id const _window_lifetime_id;
    time_editor *const _editor;
    time_editor_closer *const _closer;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae
