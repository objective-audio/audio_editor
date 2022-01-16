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

std::string time_presenter_utils::to_sign_string(ae::timing_components const &components) {
    return components.is_minus() ? "-" : "+";
}

std::string time_presenter_utils::to_hours_string(ae::timing_components const &components) {
    std::stringstream stream;
    stream << std::setfill('0') << std::right << std::setw(2) << static_cast<uint16_t>(components.hours());
    return stream.str();
}

std::string time_presenter_utils::to_minutes_string(ae::timing_components const &components) {
    std::stringstream stream;
    stream << std::setfill('0') << std::right << std::setw(2) << static_cast<uint16_t>(components.minutes());
    return stream.str();
}

std::string time_presenter_utils::to_seconds_string(ae::timing_components const &components) {
    std::stringstream stream;
    stream << std::setfill('0') << std::right << std::setw(2) << static_cast<uint16_t>(components.seconds());
    return stream.str();
}

std::string time_presenter_utils::to_fraction_string(ae::timing_components const &components,
                                                     uint32_t const fraction_digits) {
    std::stringstream stream;
    if (fraction_digits > 0) {
        stream << std::setfill('0') << std::right << std::setw(fraction_digits) << components.fraction();
    }
    return stream.str();
}

std::string time_presenter_utils::time_text(frame_index_t const frame,
                                            std::shared_ptr<timing_for_time_presenter> const &timing) {
    auto const components = timing->components(frame);

    std::stringstream stream;

    stream << to_sign_string(components);
    stream << to_hours_string(components);
    stream << ":";
    stream << to_minutes_string(components);
    stream << ":";
    stream << to_seconds_string(components);

    auto const fraction_string = to_fraction_string(components, timing->fraction_digits());

    if (!fraction_string.empty()) {
        stream << ".";
        stream << fraction_string;
    }

    return stream.str();
}
