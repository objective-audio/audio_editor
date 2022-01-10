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

std::string time_presenter_utils::time_text(int64_t const frame,
                                            std::shared_ptr<timing_for_time_presenter> const &timing) {
    sample_rate_t const sample_rate = timing->sample_rate();
    int64_t const abs_frame = std::abs(frame);

    frame_index_t const fraction = timing->fraction_value(frame);
    frame_index_t const sec = playing::math::floor_int(abs_frame, sample_rate) / sample_rate;
    frame_index_t const mod_sec = playing::math::mod_int(sec, 60);
    frame_index_t const min = sec / 60;
    frame_index_t const mod_min = playing::math::mod_int(min, 60);
    frame_index_t const hour = min / 60;

    std::stringstream stream;

    stream << std::to_string(hour) << ":";
    stream << std::setw(2) << std::setfill('0') << mod_min;
    stream << ":";
    stream << std::setw(2) << std::setfill('0') << mod_sec;

    auto const digits = timing->fraction_digits();

    if (digits > 0) {
        stream << ".";
        stream << std::setw(digits) << std::setfill('0') << fraction;
    }

    return stream.str();
}
