//
//  ae_action_normal_routing.h
//

#pragma once

#include <audio_editor_core/ae_action_router_dependency.h>

#include <memory>

namespace yas::ae {
struct action_normal_routing : action_routing {
    [[nodiscard]] static std::shared_ptr<action_normal_routing> make_shared();

    [[nodiscard]] std::optional<ae::action> to_action(ae::key const) override;

   private:
    action_normal_routing();
};
}  // namespace yas::ae
