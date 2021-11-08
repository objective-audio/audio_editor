//
//  ae_pinch_gesture_controller.h
//

#pragma once

#include <audio_editor_core/ae_pinch_gesture_controller_dependency.h>

#include <memory>
#include <optional>

namespace yas::ae {
class pinch_gesture;

struct pinch_gesture_controller {
    [[nodiscard]] static std::shared_ptr<pinch_gesture_controller> make_shared(std::string const &project_id);

    void handle_gesture(pinch_gesture const &);

   private:
    std::weak_ptr<zooming_for_pinch_gesture_controller> const _zooming;

    pinch_gesture_controller(std::shared_ptr<zooming_for_pinch_gesture_controller> const &);
};
}  // namespace yas::ae
