//
//  ae_window_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ae {
struct window_receiver final : action_receivable {
    [[nodiscard]] static std::shared_ptr<window_receiver> make_shared(window_lifetime_id const &);
    window_receiver(window_lifetime_id const &);

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &, ae::action_id const &) const override;
    void handle_action(ae::action const &) const override;
    action_receivable_state receivable_state(ae::action const &) const override;

   private:
    window_lifetime_id const _lifetime_id;
};
}  // namespace yas::ae
