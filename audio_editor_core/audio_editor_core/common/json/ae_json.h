//
//  ae_json.h
//

#pragma once

#include <audio_editor_core/ae_json_value.h>

namespace yas::ae {
std::string to_json_string(json_value const &);
json_value to_json_value(std::string const &);
}  // namespace yas::ae
