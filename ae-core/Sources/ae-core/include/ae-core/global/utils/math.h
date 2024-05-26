//
//  math.h
//

#pragma once

#include <cstddef>
#include <cstdint>

namespace yas::ae::math {
uint32_t decimal_digits_from_size(std::size_t const size);
uint32_t decimal_max_size_of_digits(uint32_t const);
}  // namespace yas::ae::math
