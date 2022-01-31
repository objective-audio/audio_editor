//
//  ae_action_time_routing.h
//

#pragma once

#include <audio_editor_core/ae_action_router_dependency.h>

#include <memory>

namespace yas::ae {
struct action_time_routing : action_routing {
    [[nodiscard]] static std::shared_ptr<action_time_routing> make_shared();

    [[nodiscard]] std::optional<ae::action> to_action(ae::key const) override;
    [[nodiscard]] bool responds_to_action(ae::action_kind const) override;

   private:
    action_time_routing();
};
}  // namespace yas::ae
