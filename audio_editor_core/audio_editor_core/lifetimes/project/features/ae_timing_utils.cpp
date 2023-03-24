//
//  ae_timing_utils.cpp
//

#include "ae_timing_utils.h"

#include <audio_editor_core/ae_math.h>
#include <playing/yas_playing_math.h>

using namespace yas;
using namespace yas::ae;

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

uint32_t timing_utils::to_fraction_digits(timing_fraction_kind const kind, sample_rate_t const sample_rate) {
    auto const unit = to_fraction_unit(kind, sample_rate);
    return math::decimal_digits_from_size(unit);
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

    auto const fraction_unit = to_fraction_unit(kind, sample_rate);
    auto const fraction = to_fraction_value(frame, kind, sample_rate);
    auto const sec = static_cast<uint32_t>(playing::math::floor_int(abs_frame, sample_rate) / sample_rate);
    auto const min = sec / 60;

    return ae::timing_components{{.is_minus = frame < 0,
                                  .hours = static_cast<uint8_t>(min / 60),
                                  .minutes = static_cast<uint8_t>(playing::math::mod_int(min, 60)),
                                  .seconds = static_cast<uint8_t>(playing::math::mod_int(sec, 60)),
                                  .fraction = fraction,
                                  .fraction_unit_size = fraction_unit}};
}

timing_components timing_utils::to_floored_components(frame_index_t const frame, timing_unit_kind const unit_kind,
                                                      timing_fraction_kind const fraction_kind,
                                                      sample_rate_t const sample_rate) {
    auto const source = to_components(frame, fraction_kind, sample_rate);

    timing_components::args floored_args{.is_minus = source.is_minus(),
                                         .hours = 0,
                                         .minutes = 0,
                                         .seconds = 0,
                                         .fraction = 0,
                                         .fraction_unit_size = source.fraction_unit_size()};

    switch (unit_kind) {
        case timing_unit_kind::fraction:
            floored_args.fraction = source.value(timing_unit_kind::fraction);
        case timing_unit_kind::seconds:
            floored_args.seconds = source.value(timing_unit_kind::seconds);
        case timing_unit_kind::minutes:
            floored_args.minutes = source.value(timing_unit_kind::minutes);
        case timing_unit_kind::hours:
            floored_args.hours = source.value(timing_unit_kind::hours);
    }

    timing_components floored{floored_args};

    if (frame >= 0 || to_frame(floored, fraction_kind, sample_rate) == frame) {
        return floored;
    } else {
        // マイナスならマイナス方向に丸める（切り捨てたframeと一致しなければ、切り捨てた値をマイナス方向に一つずらす）
        auto const offset = timing_components::offset({.is_minus = true,
                                                       .count = 1,
                                                       .unit_index = to_index(unit_kind),
                                                       .fraction_unit_size = source.fraction_unit_size()});
        return floored.adding(offset);
    }
}

frame_index_t timing_utils::to_frame(timing_components const &components, timing_fraction_kind const kind,
                                     sample_rate_t const sample_rate) {
    frame_index_t frame = static_cast<frame_index_t>(components.value(timing_unit_kind::hours)) * 60 * 60 * sample_rate;
    frame += static_cast<frame_index_t>(components.value(timing_unit_kind::minutes)) * 60 * sample_rate;
    frame += static_cast<frame_index_t>(components.value(timing_unit_kind::seconds)) * sample_rate;

    frame += static_cast<uint64_t>(components.value(timing_unit_kind::fraction)) * static_cast<uint64_t>(sample_rate) /
             static_cast<uint64_t>(to_fraction_unit(kind, sample_rate));

    if (components.is_minus()) {
        frame *= -1;
    }

    return frame;
}
