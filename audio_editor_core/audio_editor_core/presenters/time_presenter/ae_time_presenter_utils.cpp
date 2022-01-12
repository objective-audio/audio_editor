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

    auto const fraction = timing->fraction_value(frame);
    auto const sec = static_cast<uint32_t>(playing::math::floor_int(abs_frame, sample_rate) / sample_rate);
    auto const min = sec / 60;

    return ae::time_components{.is_minus = frame < 0,
                               .hours = static_cast<uint8_t>(min / 60),
                               .minutes = static_cast<uint8_t>(playing::math::mod_int(min, 60)),
                               .seconds = static_cast<uint8_t>(playing::math::mod_int(sec, 60)),
                               .fraction = fraction,
                               .fraction_digits = timing->fraction_digits()};
}

std::string time_presenter_utils::time_text(frame_index_t const frame,
                                            std::shared_ptr<timing_for_time_presenter> const &timing) {
    auto const components = time_components(frame, timing);

    std::stringstream stream;

    stream << components.sign_string();
    stream << components.hours_string();
    stream << ":";
    stream << components.minutes_string();
    stream << ":";
    stream << components.seconds_string();

    auto const fraction_string = components.fraction_string();

    if (!fraction_string.empty()) {
        stream << ".";
        stream << fraction_string;
    }

    return stream.str();
}
