//
//  ae_action_sender.h
//

#pragma once

#include <memory>
#include <optional>
#include <vector>

namespace yas::ae {
enum class key;
enum class responding;
class action;
class action_id;
class action_receiver_providable;

struct action_sender final {
    [[nodiscard]] static std::shared_ptr<action_sender> make_shared(action_receiver_providable *root_provider);

    action_sender(action_receiver_providable *root_provider);

    [[nodiscard]] std::optional<ae::action> to_action(ae::key const &, ae::action_id const &) const;
    void handle_action(ae::action const &);
    [[nodiscard]] responding responding_to_action(ae::action const &);

   private:
    action_receiver_providable *const _root_provider;
};
}  // namespace yas::ae