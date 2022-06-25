//
//  ae_project_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class project_editor_responder;

struct project_receiver final : action_receivable {
    [[nodiscard]] static std::shared_ptr<project_receiver> make_shared(project_id const &, project_editor_responder *);

    project_receiver(project_id const &, project_editor_responder *);

   private:
    project_id const _project_id;
    project_editor_responder *const _responder;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae
