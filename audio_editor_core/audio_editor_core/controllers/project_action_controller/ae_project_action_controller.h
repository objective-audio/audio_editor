//
//  ae_project_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <audio_editor_core/ae_project_id.h>

namespace yas::ae {
class action_sender;

struct project_action_controller final {
    [[nodiscard]] static std::shared_ptr<project_action_controller> make_shared(project_id const &, action_sender *);

    project_action_controller(project_id const &, action_sender *);

    void handle_action(action const &);
    void handle_action(action_kind const &, std::string const &value = "");
    void handle_key(ae::key const);

   private:
    project_id const _project_id;
    action_sender *const _action_sender;
};
}  // namespace yas::ae
