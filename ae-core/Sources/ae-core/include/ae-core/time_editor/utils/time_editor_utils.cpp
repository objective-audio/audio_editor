//
//  time_editor_utils.cpp
//

#include "time_editor_utils.h"

#include <ae-core/global/utils/math.h>

using namespace yas;
using namespace yas::ae;

number_components time_editor_utils::to_editing_components(number_components const &source) {
    std::vector<number_components_unit> units;

    for (auto const &src_unit : source.units()) {
        auto const digits = math::decimal_digits_from_size(src_unit.size);
        auto const size = math::decimal_max_size_of_digits(digits);
        units.emplace_back(number_components_unit{.size = size, .value = src_unit.value});
    }

    return {source.is_minus(), std::move(units)};
}
