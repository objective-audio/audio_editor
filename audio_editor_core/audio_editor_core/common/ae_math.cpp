//
//  ae_math.cpp
//

#include "ae_math.h"

#include <cmath>
#include <stdexcept>

using namespace yas;
using namespace yas::ae;

uint32_t math::fraction_digits(uint32_t const fraction_size) {
    if (fraction_size == 0) {
        throw std::invalid_argument("ae::math::fraction_digits - size is zero.");
    }

    uint32_t value = fraction_size - 1;
    uint32_t digits = 0;

    while (value != 0) {
        value = value / 10;
        ++digits;
    }

    return digits;
}
