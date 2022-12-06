//
//  ae_window_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <memory>

namespace yas::ae {
struct window_receiver final : action_receivable {
    window_receiver(window_lifetime_id const &);

    [[nodiscard]] std::optional<action_id> receivable_id() const override;
    [[nodiscard]] std::optional<ae::action> to_action(ae::key const &) const override;
    void receive(ae::action const &) const override;
    [[nodiscard]] action_receivable_state receivable_state(ae::action const &) const override;

   private:
    window_lifetime_id const _lifetime_id;

    window_receiver(window_receiver const &) = delete;
    window_receiver(window_receiver &&) = delete;
    window_receiver &operator=(window_receiver const &) = delete;
    window_receiver &operator=(window_receiver &&) = delete;
};
}  // namespace yas::ae
