//
//  ae_window_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>

#include <memory>

namespace yas::ae {
struct window_receiver final : action_receivable {
    [[nodiscard]] static std::shared_ptr<window_receiver> make_shared();
    window_receiver();

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae
