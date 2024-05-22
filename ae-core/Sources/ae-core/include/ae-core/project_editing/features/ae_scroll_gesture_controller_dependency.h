//
//  ae_scroll_gesture_controller_dependency.h
//

#pragma once

namespace yas::ae {
struct horizontal_scrolling_for_gesture_controller {
    virtual ~horizontal_scrolling_for_gesture_controller() = default;

    virtual void begin() = 0;
    virtual void set_delta_time(double const) = 0;
    virtual void end() = 0;
};
}  // namespace yas::ae
