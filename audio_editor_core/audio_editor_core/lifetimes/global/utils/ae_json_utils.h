//
//  ae_json_utils.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>

#include <string>

namespace yas {
std::string to_json_string(ae::time::range const &);
std::optional<ae::time::range> to_time_range(std::string const &);
}  // namespace yas
