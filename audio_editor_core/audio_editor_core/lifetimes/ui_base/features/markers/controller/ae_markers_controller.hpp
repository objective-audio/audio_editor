//
//  ae_markers_controller.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>

#include <audio_editor_core/ae_marker_index.hpp>

namespace yas::ae {
class project_action_sender;

struct markers_controller final {
    [[nodiscard]] static std::shared_ptr<markers_controller> make_shared(project_lifetime_id const &);

    markers_controller(std::shared_ptr<project_action_sender> const &);

    void select(std::vector<marker_index> const &);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;

    markers_controller(markers_controller const &) = delete;
    markers_controller(markers_controller &&) = delete;
    markers_controller &operator=(markers_controller const &) = delete;
    markers_controller &operator=(markers_controller &&) = delete;
};
}  // namespace yas::ae
