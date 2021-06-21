//
//  ae_keyboard_utils.h
//

#pragma once

#include <audio_editor_core/ae_keyboard_types.h>

#include <cstdint>
#include <optional>

namespace yas::ae::keyboard_utils {
std::optional<ae::key> to_key(uint16_t const key_code);
}
