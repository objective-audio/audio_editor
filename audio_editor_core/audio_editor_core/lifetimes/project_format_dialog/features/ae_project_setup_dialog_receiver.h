//
//  ae_project_setup_dialog_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

namespace yas::ae {
struct project_setup_dialog_receiver final : action_receivable {
    [[nodiscard]] static std::shared_ptr<project_setup_dialog_receiver> make_shared(window_lifetime_id const &);

    project_setup_dialog_receiver(window_lifetime_id const &);

   private:
    window_lifetime_id const _window_lifetime_id;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae
