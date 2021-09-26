//
//  ae_ui_button_utils.h
//

#pragma once

#include <cstddef>

namespace yas::ae {
enum class ui_button_state : std::size_t {
    disabled,
    enabled,
};

namespace ui_button_utils {
    std::size_t to_state_idx(ui_button_state const);
}
}  // namespace yas::ae
