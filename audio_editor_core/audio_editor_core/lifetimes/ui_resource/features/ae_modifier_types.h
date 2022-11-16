//
//  ae_modifier_types.h
//

#pragma once

#include <audio_editor_core/ae_modifier.h>

namespace yas::ae {
enum class modifier_event_state {
    began,
    ended,
};

struct modifier_event final {
    modifier_event_state state;
    modifier modifier;
};
}  // namespace yas::ae
