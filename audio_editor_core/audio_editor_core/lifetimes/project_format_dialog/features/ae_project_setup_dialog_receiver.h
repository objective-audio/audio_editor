//
//  ae_project_setup_dialog_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>

namespace yas::ae {
struct project_setup_dialog_receiver final : action_receivable {
    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae
