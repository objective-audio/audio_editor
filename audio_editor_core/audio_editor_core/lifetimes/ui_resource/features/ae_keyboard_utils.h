//
//  ae_keyboard_utils.h
//

#pragma once

#include <audio_editor_core/ae_key.h>
#include <audio_editor_core/ae_modifier.h>
#include <ui/yas_ui_event_types.h>

#include <optional>
#include <unordered_set>

namespace yas::ae::keyboard_utils {
std::optional<key> to_key(ui::key_event const &, std::unordered_set<ae::modifier> const &);
}  // namespace yas::ae::keyboard_utils
