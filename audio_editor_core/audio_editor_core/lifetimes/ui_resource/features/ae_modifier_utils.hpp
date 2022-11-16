//
//  ae_modifier_utils.hpp
//

#pragma once

#include <audio_editor_core/ae_modifier.h>
#include <ui/yas_ui_event_types.h>

namespace yas::ae::modifier_utils {
ae::modifier to_modifier(ui::modifier_flags const flag);
}
