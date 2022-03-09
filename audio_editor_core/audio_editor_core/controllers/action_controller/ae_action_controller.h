//
//  ae_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class responder_stack;

struct action_controller final {
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared(std::string const &project_id);
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared(std::shared_ptr<responder_stack> const &);

    void handle_action(action const &);
    void handle_key(ae::key const);

   private:
    std::weak_ptr<responder_stack> const _responder_stack;
    observing::notifier_ptr<url> const _export_notifier;

    action_controller(std::shared_ptr<responder_stack> const &);
};
}  // namespace yas::ae
