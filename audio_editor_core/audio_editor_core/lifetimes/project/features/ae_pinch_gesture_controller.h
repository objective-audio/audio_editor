//
//  ae_pinch_gesture_controller.h
//

#pragma once

#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <audio_editor_core/ae_modifier_types.h>

#include <memory>

namespace yas::ae {
class zooming_pair;

struct pinch_gesture_controller final {
    [[nodiscard]] static std::shared_ptr<pinch_gesture_controller> make_shared(zooming_pair *);

    pinch_gesture_controller(zooming_pair *);

    void handle_gesture(pinch_gesture const &);
    void handle_modifier(modifier_event_state const &);

   private:
    zooming_pair *const _zooming_pair;
    bool _is_modified = false;

    pinch_gesture_controller(pinch_gesture_controller const &) = delete;
    pinch_gesture_controller(pinch_gesture_controller &&) = delete;
    pinch_gesture_controller &operator=(pinch_gesture_controller const &) = delete;
    pinch_gesture_controller &operator=(pinch_gesture_controller &&) = delete;
};
}  // namespace yas::ae
