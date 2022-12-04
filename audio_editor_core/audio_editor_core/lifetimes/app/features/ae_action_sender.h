//
//  ae_action_sender.h
//

#pragma once

#include <memory>
#include <optional>
#include <vector>

namespace yas::ae {
enum class key;
enum class action_receivable_state;
class action;
class action_id;
class action_receiver_providable;

struct action_sender final {
    explicit action_sender(action_receiver_providable *root_provider);

    void send(ae::action const &, ae::action_id const &);
    void send(ae::key const &, ae::action_id const &);
    [[nodiscard]] action_receivable_state receivable_state(ae::action const &, ae::action_id const &);

   private:
    action_receiver_providable *const _root_provider;
};
}  // namespace yas::ae
