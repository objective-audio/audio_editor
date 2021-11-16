//
//  ae_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct action_controller final {
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared();

    void handle_action(action const);
    void export_to_file(url const &);

    [[nodiscard]] observing::endable observe_action(std::function<void(action const &)> &&);
    [[nodiscard]] observing::endable observe_export(std::function<void(url const &)> &&);

   private:
    observing::notifier_ptr<action> const _action_notifier;
    observing::notifier_ptr<url> const _export_notifier;

    action_controller();
};
}  // namespace yas::ae
