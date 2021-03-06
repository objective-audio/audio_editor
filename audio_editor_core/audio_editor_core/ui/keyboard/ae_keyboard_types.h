//
//  ae_keyboard_types.h
//

#pragma once

namespace yas::ae {
enum class key {
    space,
    a,
    s,
    d,
    f,
    n,
    shift_n,
    t,
    del,
    esc,
    left,
    shift_left,
    right,
    shift_right,
    up,
    down,
    plus,
    hyphen,
    ret,
    tab,
    shift_tab,
    left_bracket,
    right_bracket,

    num_0,
    num_1,
    num_2,
    num_3,
    num_4,
    num_5,
    num_6,
    num_7,
    num_8,
    num_9,
};

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
