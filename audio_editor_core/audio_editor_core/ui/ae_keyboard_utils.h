//
//  ae_keyboard_utils.h
//

#pragma once

#include <audio_editor_core/ae_keyboard_types.h>
#include <ui/yas_ui_event_types.h>

#include <cstdint>
#include <optional>

namespace yas::ae::keyboard_utils {
std::optional<key> to_key(uint16_t const key_code);
ae::modifier to_modifier(ui::modifier_flags const flag);
}  // namespace yas::ae::keyboard_utils
