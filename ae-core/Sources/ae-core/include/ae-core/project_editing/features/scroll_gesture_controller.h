//
//  scroll_gesture_controller.h
//

#pragma once

#include <ae-core/global/value_types/gesture.h>
#include <ae-core/project_editing/features/scroll_gesture_controller_dependency.h>

#include <memory>

namespace yas::ae {
class vertical_scrolling;
class zooming_pair;

struct scroll_gesture_controller final {
    scroll_gesture_controller(horizontal_scrolling_for_gesture_controller *, vertical_scrolling *,
                              zooming_pair const *);

    void handle_gesture(scroll_gesture const &);

   private:
    horizontal_scrolling_for_gesture_controller *const _horizontal_scrolling;
    vertical_scrolling *const _vertical_scrolling;
    zooming_pair const *const _zooming_pair;

    scroll_gesture_controller(scroll_gesture_controller const &) = delete;
    scroll_gesture_controller(scroll_gesture_controller &&) = delete;
    scroll_gesture_controller &operator=(scroll_gesture_controller const &) = delete;
    scroll_gesture_controller &operator=(scroll_gesture_controller &&) = delete;
};
}  // namespace yas::ae
