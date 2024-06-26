//
//  gesture.h
//

#pragma once

#include <ui/common/yas_ui_types.h>

namespace yas::ae {
enum class gesture_state {
    began,
    changed,
    ended,
};

struct pinch_gesture final {
    gesture_state state;
    double magnification;
};

struct scroll_gesture final {
    gesture_state state;
    double delta_x;
    double delta_y;
};
}  // namespace yas::ae

namespace yas::ui {
enum class event_phase;
}

namespace yas {
ae::gesture_state to_gesture_state(ui::event_phase const);
}
