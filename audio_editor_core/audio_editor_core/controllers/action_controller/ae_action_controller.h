//
//  ae_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_action_controller_dependency.h>

#include <memory>

namespace yas::ae {
struct action_controller {
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared(std::string const &project_id);

    void handle_action(action const);

   private:
    std::weak_ptr<project_editor_for_action_controller> const _project_editor;

    action_controller(std::shared_ptr<project_editor_for_action_controller> const &);
};
}  // namespace yas::ae
