//
//  ae_time_presenter_utils.cpp
//

#include "ae_time_presenter_utils.h"

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_math.h>
#include <playing/yas_playing_math.h>

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

time_components time_presenter_utils::time_components(frame_index_t const frame,
                                                      std::shared_ptr<timing_for_time_presenter> const &timing) {
    sample_rate_t const sample_rate = timing->sample_rate();
    frame_index_t const abs_frame = std::abs(frame);

    uint32_t const fraction = timing->fraction_value(frame);
    uint32_t const sec = static_cast<uint32_t>(playing::math::floor_int(abs_frame, sample_rate) / sample_rate);
    uint32_t const mod_sec = static_cast<uint32_t>(playing::math::mod_int(sec, 60));
    uint32_t const min = sec / 60;
    uint32_t const mod_min = static_cast<uint32_t>(playing::math::mod_int(min, 60));
    uint32_t const hour = min / 60;

    return ae::time_components{.is_minus = frame < 0,
                               .hour = hour,
                               .minute = mod_min,
                               .second = mod_sec,
                               .fraction = fraction,
                               .fraction_digits = timing->fraction_digits()};
}

std::string time_presenter_utils::time_text(frame_index_t const frame,
                                            std::shared_ptr<timing_for_time_presenter> const &timing) {
    auto const components = time_components(frame, timing);

    std::stringstream stream;

    stream << std::to_string(components.hour) << ":";
    stream << std::setw(2) << std::setfill('0') << components.minute;
    stream << ":";
    stream << std::setw(2) << std::setfill('0') << components.second;

    auto const digits = components.fraction_digits;

    if (digits > 0) {
        stream << ".";
        stream << std::setw(digits) << std::setfill('0') << components.fraction;
    }

    return stream.str();
}
