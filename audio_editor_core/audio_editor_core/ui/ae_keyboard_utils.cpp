//
//  ae_keyboard_utils.cpp
//

#include "ae_keyboard_utils.h"

using namespace yas;
using namespace yas::ae;

std::optional<ae::key> keyboard_utils::to_key(uint16_t const key_code) {
    switch (key_code) {
        case 49:
            return ae::key::space;

        default:
            return std::nullopt;
    }
}
