//
//  ae_time_presenter_utils.cpp
//

#include "ae_time_presenter_utils.h"

#include <playing/yas_playing_math.h>

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

int time_presenter_utils::after_point_digits(uint32_t const sample_rate) {
    if (sample_rate == 0) {
        throw std::invalid_argument("sample_rate is zero.");
    }

    if (sample_rate == 1) {
        return 0;
    }

    uint32_t value = 10;
    uint32_t digits = 1;
    uint32_t prev_mod = sample_rate;

    while (true) {
        if ((sample_rate / value) == 0) {
            if (prev_mod == 0) {
                digits -= 1;
            }
            break;
        }

        prev_mod = sample_rate % value;

        digits += 1;
        value *= 10;
    }

    return static_cast<int>(digits);
}

std::string time_presenter_utils::time_text(int64_t const frame, uint32_t const sample_rate) {
    int64_t const abs_frame = std::abs(frame);

    auto const digits = after_point_digits(sample_rate);

    int64_t const after_point = playing::math::mod_int(abs_frame, sample_rate);
    int64_t const sec = playing::math::floor_int(abs_frame, sample_rate) / sample_rate;
    int64_t const mod_sec = playing::math::mod_int(sec, 60);
    int64_t const min = sec / 60;
    int64_t const mod_min = playing::math::mod_int(min, 60);
    int64_t const hour = min / 60;

    std::stringstream stream;

    stream << std::to_string(hour) << ":";
    stream << std::setw(2) << std::setfill('0') << mod_min;
    stream << ":";
    stream << std::setw(2) << std::setfill('0') << mod_sec;

    if (digits > 0) {
        stream << ".";
        stream << std::setw(digits) << std::setfill('0') << after_point;
    }

    return stream.str();
}
