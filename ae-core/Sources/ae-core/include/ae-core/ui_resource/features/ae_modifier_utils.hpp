//
//  ae_modifier_utils.hpp
//

#pragma once

#include <ae-core/ui_resource/features/ae_modifier.h>
#include <ui/event/yas_ui_event_types.h>

namespace yas::ae::modifier_utils {
ae::modifier to_modifier(ui::modifier_flags const flag);
}
