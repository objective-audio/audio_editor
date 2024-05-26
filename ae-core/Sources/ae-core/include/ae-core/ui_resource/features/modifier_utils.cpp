//
//  modifier_utils.cpp
//

#include "modifier_utils.hpp"

using namespace yas;
using namespace yas::ae;

ae::modifier modifier_utils::to_modifier(ui::modifier_flags const flag) {
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
