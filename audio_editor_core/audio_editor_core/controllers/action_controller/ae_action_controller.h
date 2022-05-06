//
//  ae_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class responder_stack;

struct action_controller final {
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared(project_id const &project_id);

    void handle_action(action const &);
    void handle_key(ae::key const);

   private:
    std::weak_ptr<responder_stack> const _responder_stack;

    action_controller(std::shared_ptr<responder_stack> const &);
};
}  // namespace yas::ae
