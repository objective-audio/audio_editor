//
//  ae_pinch_gesture_controller.h
//

#pragma once

#include <memory>
#include <optional>

namespace yas::ae {
class project;
class pinch_gesture;

struct pinch_gesture_controller {
    [[nodiscard]] static std::shared_ptr<pinch_gesture_controller> make_shared(std::string const &project_id);

    void handle_gesture(pinch_gesture const &);

   private:
    std::weak_ptr<project> const _project;

    pinch_gesture_controller(std::shared_ptr<project> const &);
};
}  // namespace yas::ae
