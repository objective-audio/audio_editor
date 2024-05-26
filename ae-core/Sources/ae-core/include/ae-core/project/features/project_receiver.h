//
//  project_receiver.h
//

#pragma once

#include <ae-core/global/interfaces/action_receivable.h>
#include <ae-core/global/value_types/project_lifetime_id.h>

#include <memory>

namespace yas::ae {
struct project_receiver final : action_receivable {
    project_receiver(project_lifetime_id const &);

    [[nodiscard]] std::optional<action_id> receivable_id() const override;
    [[nodiscard]] std::optional<ae::action> to_action(ae::key const &) const override;
    void receive(ae::action const &) const override;
    [[nodiscard]] action_receivable_state receivable_state(ae::action const &) const override;

   private:
    project_lifetime_id const _lifetime_id;

    project_receiver(project_receiver const &) = delete;
    project_receiver(project_receiver &&) = delete;
    project_receiver &operator=(project_receiver const &) = delete;
    project_receiver &operator=(project_receiver &&) = delete;
};
}  // namespace yas::ae
