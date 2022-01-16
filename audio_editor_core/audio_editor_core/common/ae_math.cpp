//
//  ae_math.cpp
//

#include "ae_math.h"

#include <cmath>
#include <stdexcept>

using namespace yas;
using namespace yas::ae;

uint32_t math::to_decimal_digits(uint32_t const size) {
    if (size == 0) {
        throw std::invalid_argument("ae::math::to_digits - size is zero.");
    }

    uint32_t value = size - 1;
    uint32_t digits = 0;

    while (value != 0) {
        value = value / 10;
        ++digits;
    }

    return digits;
}
