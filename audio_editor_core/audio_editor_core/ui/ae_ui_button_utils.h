//
//  ae_ui_button_utils.h
//

#pragma once

#include <ui/yas_ui_event.h>

#include <set>

namespace yas::ae {
enum class ui_button_state : std::size_t {
    disabled,
    enabled,
};

namespace ui_button_utils {
    std::size_t to_state_idx(ui_button_state const);

    std::function<bool(std::shared_ptr<ui::event> const &)> is_touch_accepted(std::set<ui::touch_id> &&);
    std::string to_event_string(ui::point const &);
    ui::point to_position_from_event_string(std::string const &);
}  // namespace ui_button_utils
}  // namespace yas::ae
