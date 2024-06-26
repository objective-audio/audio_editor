//
//  time_numbers_presenter_utils.cpp
//

#include "time_numbers_presenter_utils.h"

#include <ae-core/global/utils/math.h>
#include <ae-core/global/value_types/common_types.h>
#include <audio-playing/common/math.h>
#include <cpp-utils/fast_each.h>

#include <iomanip>
#include <sstream>

using namespace yas;
using namespace yas::ae;

std::string time_numbers_presenter_utils::to_sign_string(ae::number_components const &components) {
    return components.is_minus() ? "-" : "+";
}

std::string time_numbers_presenter_utils::to_string(number_components_unit const &unit) {
    std::stringstream stream;

    if (unit.size > 0) {
        auto const digits = math::decimal_digits_from_size(unit.size);
        stream << std::setfill('0') << std::right << std::setw(digits) << unit.value;
    } else {
        stream << unit.value;
    }

    return stream.str();
}

std::string time_numbers_presenter_utils::time_text(number_components const &components) {
    std::stringstream stream;

    stream << to_sign_string(components);

    auto each = make_fast_each(components.size());
    while (yas_each_next(each)) {
        auto const &idx = components.size() - yas_each_index(each) - 1;

        stream << to_string(components.unit(idx));

        if (idx > 1) {
            stream << ":";
        } else if (idx == 1) {
            stream << ".";
        }
    }

    return stream.str();
}

index_range time_numbers_presenter_utils::to_time_text_range(number_components const &components,
                                                             std::size_t const target_unit_idx) {
    if (target_unit_idx >= components.size()) {
        throw std::out_of_range("out of range target_unit_idx.");
    }

    // sign
    std::size_t text_idx = 1;

    auto each = make_fast_each(components.size());
    while (yas_each_next(each)) {
        auto const unit_idx = components.size() - yas_each_index(each) - 1;
        auto const &unit = components.unit(unit_idx);
        auto const digits = math::decimal_digits_from_size(unit.size);

        if (unit_idx == target_unit_idx) {
            return index_range{.index = text_idx, .length = digits};
        }

        // digits + separator
        text_idx += digits + 1;
    }

    throw std::runtime_error("unreachable.");
}
