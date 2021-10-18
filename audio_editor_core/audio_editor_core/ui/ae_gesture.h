//
//  ae_gesture.h
//

#pragma once

namespace yas::ae {
enum class gesture_state {
    began,
    changed,
    ended,
};

struct pinch_gesture {
    gesture_state state;
    double magnification;
};
}  // namespace yas::ae
