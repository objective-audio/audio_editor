//
//  ae_markers_controller.hpp
//

#pragma once

#include <audio_editor_core/ae_marker_content_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <audio_editor_core/ae_marker_index.hpp>

namespace yas::ae {
class project_action_sender;

struct markers_controller final {
    [[nodiscard]] static std::shared_ptr<markers_controller> make_shared(window_lifetime_id const &);

    markers_controller(std::shared_ptr<project_action_sender> const &action_sender);

    void select_marker_with_index(marker_index const &);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
};
}  // namespace yas::ae
