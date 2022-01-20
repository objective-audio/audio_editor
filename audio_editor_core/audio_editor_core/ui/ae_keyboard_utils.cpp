//
//  ae_keyboard_utils.cpp
//

#include "ae_keyboard_utils.h"

using namespace yas;
using namespace yas::ae;

std::optional<key> keyboard_utils::to_key(ui::key_event const &event,
                                          std::unordered_set<ae::modifier> const &modifiers) {
    switch (event.key_code()) {
        case 49:
            return key::space;
        case 51:
            return key::del;

        case 0:
            return key::a;
        case 1:
            return key::s;
        case 2:
            return key::d;
        case 3:
            return key::f;
        case 15:
            return key::r;

        case 29:
            return key::num_0;
        case 18:
            return key::num_1;
        case 19:
            return key::num_2;
        case 20:
            return key::num_3;
        case 21:
            return key::num_4;
        case 23:
            return key::num_5;
        case 22:
            return key::num_6;
        case 26:
            return key::num_7;
        case 28:
            return key::num_8;
        case 25:
            return key::num_9;

        default:
            return std::nullopt;
    }
}

ae::modifier keyboard_utils::to_modifier(ui::modifier_flags const flag) {
    switch (flag) {
        case ui::modifier_flags::alpha_shift:
            return ae::modifier::alpha_shift;
        case ui::modifier_flags::shift:
            return ae::modifier::shift;
        case ui::modifier_flags::control:
            return ae::modifier::control;
        case ui::modifier_flags::alternate:
            return ae::modifier::alternate;
        case ui::modifier_flags::command:
            return ae::modifier::command;
        case ui::modifier_flags::numeric_pad:
            return ae::modifier::numeric_pad;
        case ui::modifier_flags::help:
            return ae::modifier::help;
        case ui::modifier_flags::function:
            return ae::modifier::function;
    }
}
