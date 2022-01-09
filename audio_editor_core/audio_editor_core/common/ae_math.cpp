//
//  ae_math.cpp
//

#include "ae_math.h"

#include <stdexcept>

using namespace yas;
using namespace yas::ae;

uint32_t math::fraction_digits(uint32_t const max) {
    if (max == 0) {
        throw std::invalid_argument("sample_rate is zero.");
    }

    if (max == 1) {
        return 0;
    }

    uint32_t value = 10;
    uint32_t digits = 1;
    uint32_t prev_mod = max;

    while (true) {
        if ((max / value) == 0) {
            if (prev_mod == 0) {
                digits -= 1;
            }
            break;
        }

        prev_mod = max % value;

        digits += 1;
        value *= 10;
    }

    return digits;
}
