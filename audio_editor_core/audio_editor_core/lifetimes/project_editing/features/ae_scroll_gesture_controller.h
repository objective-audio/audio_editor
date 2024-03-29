//
//  ae_scroll_gesture_controller.h
//

#pragma once

#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_scroll_gesture_controller_dependency.h>

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
