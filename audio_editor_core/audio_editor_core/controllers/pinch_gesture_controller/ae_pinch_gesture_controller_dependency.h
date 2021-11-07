//
//  ae_pinch_gesture_controller_dependency.h
//

#pragma once

namespace yas::ae {
struct zooming_for_pinch_gesture_controller {
    virtual ~zooming_for_pinch_gesture_controller() = default;

    virtual void begin() = 0;
    virtual void set_magnification(double const) = 0;
    virtual void end() = 0;
};
}  // namespace yas::ae
