//
//  modifiers_holder_event.h
//

#pragma once

#include <ae-core/ui_resource/features/modifier.h>

namespace yas::ae {
enum class modifiers_holder_event_state {
    began,
    ended,
};

struct modifiers_holder_event final {
    modifiers_holder_event_state state;
    modifier modifier;
};
}  // namespace yas::ae
