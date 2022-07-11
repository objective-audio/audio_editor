//
//  ae_action_receivable.h
//

#pragma once

#include <audio_editor_core/ae_action_receivable_state.h>
#include <audio_editor_core/ae_keyboard_types.h>

#include <optional>

namespace yas::ae {
class action;
class action_id;

struct action_receivable {
    virtual ~action_receivable() = default;

    [[nodiscard]] virtual std::optional<action_id> receivable_id() const = 0;

    [[nodiscard]] virtual std::optional<ae::action> to_action(ae::key const &, ae::action_id const &) const = 0;
    virtual void receive(ae::action const &) const = 0;
    [[nodiscard]] virtual action_receivable_state receivable_state(ae::action const &) const = 0;
};
}  // namespace yas::ae
