//
//  ae_ui_event_utils.h
//

#pragma once

#include <ui/event/yas_ui_event.h>

namespace yas::ae::ui_event_utils {
std::string to_event_string(ui::point const &);
ui::point to_position_from_event_string(std::string const &);
}  // namespace yas::ae::ui_event_utils
