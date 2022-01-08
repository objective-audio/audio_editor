//
//  ae_scroll_gesture_controller.h
//

#pragma once

#include <audio_editor_core/ae_gesture.h>
#include <audio_editor_core/ae_scroll_gesture_controller_dependency.h>

#include <memory>

namespace yas::ae {
struct scroll_gesture_controller final {
    [[nodiscard]] static std::shared_ptr<scroll_gesture_controller> make_shared(std::string const &project_id);

    void handle_gesture(scroll_gesture const &);

   private:
    std::weak_ptr<scrolling_for_gesture_controller> const _scrolling;

    scroll_gesture_controller(std::shared_ptr<scrolling_for_gesture_controller> const &);

    scroll_gesture_controller(scroll_gesture_controller const &) = delete;
    scroll_gesture_controller(scroll_gesture_controller &&) = delete;
    scroll_gesture_controller &operator=(scroll_gesture_controller const &) = delete;
    scroll_gesture_controller &operator=(scroll_gesture_controller &&) = delete;
};
}  // namespace yas::ae
