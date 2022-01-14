//
//  ae_timing_utils.cpp
//

#include "ae_timing_utils.h"

#include <audio_editor_core/ae_math.h>
#include <playing/yas_playing_math.h>

using namespace yas;
using namespace yas::ae;

uint32_t timing_utils::to_fraction_digits(timing_fraction_kind const kind, sample_rate_t const sample_rate) {
    auto const unit = to_fraction_unit(kind, sample_rate);
    return math::fraction_digits(unit);
}

uint32_t timing_utils::to_fraction_unit(timing_fraction_kind const kind, sample_rate_t const sample_rate) {
    switch (kind) {
        case timing_fraction_kind::sample:
            return sample_rate;
        case timing_fraction_kind::milisecond:
            return 1000;
        case timing_fraction_kind::frame30:
            return 30;
    }
}

uint32_t timing_utils::to_fraction_value(frame_index_t const frame, timing_fraction_kind const kind,
                                         sample_rate_t const sample_rate) {
    uint64_t const mod_frame = std::abs(frame) % sample_rate;
    uint64_t const unit = to_fraction_unit(kind, sample_rate);
    return static_cast<uint32_t>(mod_frame * unit / static_cast<uint64_t>(sample_rate));
}

timing_components timing_utils::to_components(frame_index_t const frame, timing_fraction_kind const kind,
                                              sample_rate_t const sample_rate) {
    frame_index_t const abs_frame = std::abs(frame);

    auto const fraction = to_fraction_value(frame, kind, sample_rate);
    auto const sec = static_cast<uint32_t>(playing::math::floor_int(abs_frame, sample_rate) / sample_rate);
    auto const min = sec / 60;

    return ae::timing_components{.is_minus = frame < 0,
                                 .hours = static_cast<uint8_t>(min / 60),
                                 .minutes = static_cast<uint8_t>(playing::math::mod_int(min, 60)),
                                 .seconds = static_cast<uint8_t>(playing::math::mod_int(sec, 60)),
                                 .fraction = fraction};
}

frame_index_t timing_utils::to_frame(timing_components const &components, timing_fraction_kind const kind,
                                     sample_rate_t const sample_rate) {
    frame_index_t frame = static_cast<frame_index_t>(components.hours) * 60 * 60 * sample_rate;
    frame += static_cast<frame_index_t>(components.minutes) * 60 * sample_rate;
    frame += static_cast<frame_index_t>(components.seconds) * sample_rate;

    frame += static_cast<uint64_t>(components.fraction) * static_cast<uint64_t>(sample_rate) /
             static_cast<uint64_t>(to_fraction_unit(kind, sample_rate));

    if (components.is_minus) {
        frame *= -1;
    }

    return frame;
}

timing_components timing_utils::adding(timing_components const &lhs, timing_components const &rhs,
                                       timing_fraction_kind const kind, sample_rate_t const sample_rate) {
    if (lhs.is_zero()) {
        return rhs;
    } else if (rhs.is_zero()) {
        return lhs;
    }

    auto const unit = timing_utils::to_fraction_unit(kind, sample_rate);

    if (lhs.is_minus == rhs.is_minus) {
        uint64_t fraction = static_cast<uint64_t>(lhs.fraction) + static_cast<uint64_t>(rhs.fraction);
        uint16_t seconds = static_cast<uint16_t>(lhs.seconds) + static_cast<uint16_t>(rhs.seconds);
        uint16_t minutes = static_cast<uint16_t>(lhs.minutes) + static_cast<uint16_t>(rhs.minutes);
        uint16_t hours = static_cast<uint16_t>(lhs.hours) + static_cast<uint16_t>(rhs.hours);

        seconds += (fraction > 0) ? (fraction / unit) : 0;
        fraction %= unit;

        minutes += (seconds > 0) ? (seconds / 60) : 0;
        seconds %= 60;

        hours += (minutes > 0) ? (minutes / 60) : 0;
        minutes %= 60;

        return timing_components{.is_minus = lhs.is_minus,
                                 .hours = static_cast<uint8_t>(hours),
                                 .minutes = static_cast<uint8_t>(minutes),
                                 .seconds = static_cast<uint8_t>(seconds),
                                 .fraction = static_cast<uint32_t>(fraction)};
    } else {
        auto const abs_lhs = lhs.abs();
        auto const abs_rhs = rhs.abs();

        bool const lhs_less_than_rhs = abs_lhs < abs_rhs;
        auto const &smaller = lhs_less_than_rhs ? abs_lhs : abs_rhs;
        auto const &larger = lhs_less_than_rhs ? abs_rhs : abs_lhs;

        int64_t fraction = static_cast<int64_t>(larger.fraction) - static_cast<int64_t>(smaller.fraction);
        int16_t seconds = static_cast<int16_t>(larger.seconds) - static_cast<int16_t>(smaller.seconds);
        int16_t minutes = static_cast<int16_t>(larger.minutes) - static_cast<int16_t>(smaller.minutes);
        int16_t hours = static_cast<int16_t>(larger.hours) - static_cast<int16_t>(smaller.hours);

        if (fraction < 0) {
            fraction = unit - std::abs(fraction);
            seconds -= 1;
        }

        if (seconds < 0) {
            seconds = 60 - std::abs(seconds);
            minutes -= 1;
        }

        if (minutes < 0) {
            minutes = 60 - std::abs(minutes);
            hours -= 1;
        }

        assert(hours >= 0);

        return timing_components{.is_minus = lhs_less_than_rhs ? rhs.is_minus : lhs.is_minus,
                                 .hours = static_cast<uint8_t>(hours),
                                 .minutes = static_cast<uint8_t>(minutes),
                                 .seconds = static_cast<uint8_t>(seconds),
                                 .fraction = static_cast<uint32_t>(fraction)};
    }
}
