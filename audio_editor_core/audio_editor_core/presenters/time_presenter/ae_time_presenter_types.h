//
//  ae_time_presenter_types.h
//

#pragma once

#include <cstdint>
#include <string>

namespace yas::ae {
struct time_components {
    bool is_minus;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint32_t fraction;
    uint32_t fraction_digits;

    std::string sign_string() const;
    std::string hours_string() const;
    std::string minutes_string() const;
    std::string seconds_string() const;
    std::string fraction_string() const;
};
}  // namespace yas::ae
