//
//  ae_scroll_gesture_controller.h
//

#pragma once

#include <memory>
#include <optional>

namespace yas::ae {
class scrolling;
class scroll_gesture;

struct scroll_gesture_controller {
    [[nodiscard]] static std::shared_ptr<scroll_gesture_controller> make_shared(std::string const &project_id);

    void handle_gesture(scroll_gesture const &);

   private:
    std::weak_ptr<scrolling> const _scrolling;

    scroll_gesture_controller(std::shared_ptr<scrolling> const &);
};
}  // namespace yas::ae
