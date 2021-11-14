//
//  ae_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct action_controller final {
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared();

    void handle_action(action const);

    [[nodiscard]] observing::endable observe_action(std::function<void(action const &)> &&);

   private:
    observing::notifier_ptr<action> const _notifier;

    action_controller();
};
}  // namespace yas::ae
