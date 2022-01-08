//
//  ae_time_presenter_utils.h
//

#pragma once

#include <cstdint>
#include <string>

namespace yas::ae::time_presenter_utils {
int fraction_digits(uint32_t const max);
std::string time_text(int64_t const frame, uint32_t const sample_rate);
}  // namespace yas::ae::time_presenter_utils
