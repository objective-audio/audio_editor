//
//  ae_json_utils.h
//

#pragma once

#include <audio_editor_core/ae_json_value.h>

namespace yas::ae::json_utils {
json_value to_json_value(id object);
id to_json_object(json_value const &value);
}  // namespace yas::ae::json_utils
