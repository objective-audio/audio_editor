//
//  ae_project_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class responder_stack;

struct project_action_controller final {
    [[nodiscard]] static std::shared_ptr<project_action_controller> make_shared(responder_stack *);

    project_action_controller(responder_stack *);

    void handle_action(action const &);
    void handle_key(ae::key const);

   private:
    responder_stack *const _responder_stack;
};
}  // namespace yas::ae
