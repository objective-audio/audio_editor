//
//  ae_time_editor_receiver.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable.h>
#include <audio_editor_core/ae_project_sub_lifetime_id.h>

namespace yas::ae {
class database;
class time_editor;
class time_editor_closer;
class time_editor_responder;

struct time_editor_receiver : action_receivable {
    time_editor_receiver(project_sub_lifetime_id const &, std::shared_ptr<database> const &, time_editor *,
                         time_editor_closer *);

   private:
    project_sub_lifetime_id const _lifetime_id;
    std::weak_ptr<database> const _database;
    time_editor *const _editor;
    time_editor_closer *const _closer;

    time_editor_receiver(time_editor_receiver const &) = delete;
    time_editor_receiver(time_editor_receiver &&) = delete;
    time_editor_receiver &operator=(time_editor_receiver const &) = delete;
    time_editor_receiver &operator=(time_editor_receiver &&) = delete;

    std::optional<action_id> receivable_id() const override;
    std::optional<ae::action> to_action(ae::key const &) const override;
    void receive(ae::action const &) const override;
    action_receivable_state receivable_state(ae::action const &) const override;
};
}  // namespace yas::ae
