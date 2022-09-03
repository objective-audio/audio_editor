//
//  ae_ui_event_handling.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class project_action_sender;
class pinch_gesture_controller;
class keyboard;

struct ui_event_handling final {
    [[nodiscard]] static std::shared_ptr<ui_event_handling> make_shared(window_lifetime_id const &);

    ui_event_handling(std::shared_ptr<ui::standard> const &, std::shared_ptr<project_action_sender> const &,
                      std::shared_ptr<pinch_gesture_controller> const &, std::shared_ptr<ae::keyboard> const &);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<pinch_gesture_controller> const _pinch_gesture_controller;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae