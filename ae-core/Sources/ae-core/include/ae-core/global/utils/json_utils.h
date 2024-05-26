//
//  json_utils.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>

#include <string>

namespace yas {
std::string to_json_string(ae::time::range const &);
std::optional<ae::time::range> to_time_range(std::string const &);
}  // namespace yas
