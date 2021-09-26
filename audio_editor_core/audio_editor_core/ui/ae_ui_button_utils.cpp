//
//  ae_ui_button_utils.cpp
//

#include "ae_ui_button_utils.h"

#include <type_traits>

using namespace yas;
using namespace yas::ae;

std::size_t ui_button_utils::to_state_idx(ui_button_state const state) {
    return static_cast<std::underlying_type_t<ui_button_state>>(state);
}
