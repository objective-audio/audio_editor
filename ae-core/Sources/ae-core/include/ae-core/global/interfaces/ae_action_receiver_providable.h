//
//  ae_action_receiver_providable.h
//

#pragma once

#include <optional>
#include <vector>

namespace yas::ae {
class action_receivable;
class action_id;

struct action_receiver_providable {
    virtual ~action_receiver_providable() = default;

    [[nodiscard]] virtual std::optional<action_id> receivable_id() const = 0;
    [[nodiscard]] virtual std::vector<action_receivable *> receivers() const = 0;
    [[nodiscard]] virtual std::vector<action_receiver_providable *> sub_providers() const = 0;

    [[nodiscard]] std::vector<action_receiver_providable const *> providers(std::optional<action_id> const &) const;
    [[nodiscard]] std::vector<action_receivable const *> receivers(std::optional<action_id> const &) const;
};
}  // namespace yas::ae
