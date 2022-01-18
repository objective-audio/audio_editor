//
//  ae_action_router.h
//

#pragma once

#include <audio_editor_core/ae_action_router_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
enum class action_routing_kind {
    normal,
    time,
};

struct action_router final {
    [[nodiscard]] static std::shared_ptr<action_router> make_shared();

    void set_kind(action_routing_kind const);
    [[nodiscard]] action_routing_kind const &kind() const;
    [[nodiscard]] observing::syncable observe_kind(std::function<void(action_routing_kind const &)> &&);

    std::optional<ae::action> to_action(ae::key const);

   private:
    observing::value::holder_ptr<action_routing_kind> const _kind;
    std::shared_ptr<action_routing> _routing;

    observing::canceller_pool _pool;

    action_router();
};
}  // namespace yas::ae
