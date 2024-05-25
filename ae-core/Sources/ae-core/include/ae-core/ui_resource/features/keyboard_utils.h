//
//  keyboard_utils.h
//

#pragma once

#include <ae-core/ui_resource/features/key.h>
#include <ae-core/ui_resource/features/modifier.h>
#include <ui/event/yas_ui_event_types.h>

#include <optional>
#include <unordered_set>

namespace yas::ae::keyboard_utils {
std::optional<key> to_key(ui::key_event const &, std::unordered_set<ae::modifier> const &);
}  // namespace yas::ae::keyboard_utils
