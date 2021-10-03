//
//  ae_keyboard_utils.cpp
//

#include "ae_keyboard_utils.h"

using namespace yas;
using namespace yas::ae;

std::optional<key> keyboard_utils::to_key(uint16_t const key_code) {
    switch (key_code) {
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

        case 29:
            return key::zero;

        default:
            return std::nullopt;
    }
}
