//
//  ae_modifier_types.h
//

#pragma once

namespace yas::ae {
enum class modifier {
    alpha_shift,
    shift,
    control,
    alternate,
    command,
    numeric_pad,
    help,
    function,
};

enum class modifier_event_state {
    began,
    ended,
};

struct modifier_event final {
    modifier_event_state state;
    modifier modifier;
};
}  // namespace yas::ae
