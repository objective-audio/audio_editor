//
//  ae_json_utils.cpp
//

#include "ae_json_utils.h"

#include <cpp-utils/json.h>

namespace yas::ae::json_constants {
namespace time_range_key {
    static std::string const frame = "frame";
    static std::string const length = "length";
}  // namespace time_range_key
}  // namespace yas::ae::json_constants

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::json_constants;

std::string yas::to_json_string(time::range const &range) {
    json_value const map_value{json_map{{time_range_key::frame, json_value{range.frame}},
                                        {time_range_key::length, json_value{int64_t(range.length)}}}};
    return to_json_string(map_value);
}

std::optional<time::range> yas::to_time_range(std::string const &json_string) {
    auto const &frame_key = time_range_key::frame;
    auto const &length_key = time_range_key::length;

    auto const json_value = to_json_value(json_string);

    if (!json_value.map.has_value()) {
        return std::nullopt;
    }

    auto const &map = json_value.map.value();

    if (!map.contains(frame_key) || !map.contains(length_key)) {
        return std::nullopt;
    }

    if (!map.at(frame_key).integer.has_value() || !map.at(length_key).integer.has_value()) {
        return std::nullopt;
    }

    auto const &frame = map.at(frame_key).integer.value();
    auto const &length = map.at(length_key).integer.value();

    return time::range{frame, length_t(length)};
}
