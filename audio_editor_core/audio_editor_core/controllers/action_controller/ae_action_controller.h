//
//  ae_action_controller.h
//

#pragma once

#include <audio_editor_core/ae_action.h>

#include <memory>

namespace yas::ae {
class project;

struct action_controller {
    [[nodiscard]] static std::shared_ptr<action_controller> make_shared(std::string const &project_id);

    void handle_action(action const);

   private:
    std::weak_ptr<project> const _project;

    action_controller(std::shared_ptr<project> const &);
};
}  // namespace yas::ae
