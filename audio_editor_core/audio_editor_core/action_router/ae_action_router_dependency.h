//
//  ae_action_router_dependency.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>

#include <optional>

namespace yas::ae {
struct action_routing {
    virtual ~action_routing() = default;

    [[nodiscard]] virtual std::optional<ae::action> to_action(ae::key const) = 0;
};
}  // namespace yas::ae
