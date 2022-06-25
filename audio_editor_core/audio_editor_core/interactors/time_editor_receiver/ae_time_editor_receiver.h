//
//  ae_time_editor_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class time_editor_responder;

struct time_editor_receiver : action_receivable {
    [[nodiscard]] static std::shared_ptr<time_editor_receiver> make_shared(project_id const &, time_editor_responder *);

    time_editor_receiver(project_id const &, time_editor_responder *);

   private:
    project_id const _project_id;
    time_editor_responder *const _responder;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void handle_action(ae::action const &) const override;
    responding responding_to_action(ae::action const &) const override;
};
}  // namespace yas::ae
