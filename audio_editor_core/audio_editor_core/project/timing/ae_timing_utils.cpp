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
    return math::to_digits(unit);
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

    auto const fraction_unit = to_fraction_unit(kind, sample_rate);
    auto const fraction = to_fraction_value(frame, kind, sample_rate);
    auto const sec = static_cast<uint32_t>(playing::math::floor_int(abs_frame, sample_rate) / sample_rate);
    auto const min = sec / 60;

    return ae::timing_components{{.is_minus = frame < 0,
                                  .hours = static_cast<uint8_t>(min / 60),
                                  .minutes = static_cast<uint8_t>(playing::math::mod_int(min, 60)),
                                  .seconds = static_cast<uint8_t>(playing::math::mod_int(sec, 60)),
                                  .fraction = fraction,
                                  .fraction_unit_count = fraction_unit}};
}

frame_index_t timing_utils::to_frame(timing_components const &components, timing_fraction_kind const kind,
                                     sample_rate_t const sample_rate) {
    frame_index_t frame = static_cast<frame_index_t>(components.hours()) * 60 * 60 * sample_rate;
    frame += static_cast<frame_index_t>(components.minutes()) * 60 * sample_rate;
    frame += static_cast<frame_index_t>(components.seconds()) * sample_rate;

    frame += static_cast<uint64_t>(components.fraction()) * static_cast<uint64_t>(sample_rate) /
             static_cast<uint64_t>(to_fraction_unit(kind, sample_rate));

    if (components.is_minus()) {
        frame *= -1;
    }

    return frame;
}
