//
//  action_receivable.h
//

#pragma once

#include <ae-core/global/value_types/action_receivable_state.h>
#include <ae-core/ui_resource/features/key.h>

#include <optional>

namespace yas::ae {
class action;
class action_id;

struct action_receivable {
    virtual ~action_receivable() = default;

    [[nodiscard]] virtual std::optional<action_id> receivable_id() const = 0;

    [[nodiscard]] virtual std::optional<ae::action> to_action(ae::key const &) const = 0;
    virtual void receive(ae::action const &) const = 0;
    [[nodiscard]] virtual action_receivable_state receivable_state(ae::action const &) const = 0;
};
}  // namespace yas::ae
