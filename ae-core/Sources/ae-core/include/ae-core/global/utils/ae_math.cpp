//
//  ae_math.cpp
//

#include "ae_math.h"

#include <cpp-utils/fast_each.h>

#include <cmath>
#include <stdexcept>

using namespace yas;
using namespace yas::ae;

uint32_t math::decimal_digits_from_size(std::size_t const size) {
    if (size == 0) {
        throw std::invalid_argument("ae::math::to_digits - size is zero.");
    }

    std::size_t value = size - 1;
    uint32_t digits = 0;

    while (value != 0) {
        value = value / 10;
        ++digits;
    }

    return digits;
}

uint32_t math::decimal_max_size_of_digits(uint32_t const digits) {
    uint32_t value = 1;

    auto each = make_fast_each(digits);
    while (yas_each_next(each)) {
        value *= 10;
    }

    return value;
}
