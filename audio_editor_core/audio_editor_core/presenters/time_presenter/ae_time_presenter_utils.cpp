//
//  ae_time_presenter_utils.cpp
//

#include "ae_time_presenter_utils.h"

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_math.h>
#include <cpp_utils/yas_fast_each.h>
#include <playing/yas_playing_math.h>

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

std::string time_presenter_utils::to_sign_string(ae::timing_components const &components) {
    return components.is_minus() ? "-" : "+";
}

std::string time_presenter_utils::to_string(number_components_unit const &unit) {
    std::stringstream stream;

    if (unit.size > 0) {
        auto const digits = math::decimal_digits_from_size(unit.size);
        stream << std::setfill('0') << std::right << std::setw(digits) << unit.value;
    } else {
        stream << unit.value;
    }

    return stream.str();
}

std::string time_presenter_utils::time_text(frame_index_t const frame,
                                            std::shared_ptr<timing_for_time_presenter> const &timing) {
    auto const components = timing->components(frame);

    std::stringstream stream;

    stream << to_sign_string(components);

    auto const &raw_components = components.raw_components();
    auto each = make_fast_each(raw_components.size());
    while (yas_each_next(each)) {
        auto const &idx = raw_components.size() - yas_each_index(each) - 1;

        stream << to_string(raw_components.unit(idx));

        if (idx > 1) {
            stream << ":";
        } else if (idx == 1) {
            stream << ".";
        }
    }

    return stream.str();
}
