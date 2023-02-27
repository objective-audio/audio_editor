//
//  ae_marker_element_controller.hpp
//

#pragma once

#include <audio_editor_core/ae_marker_content_pool.h>
#include <audio_editor_core/ae_window_lifetime_id.h>

#include <audio_editor_core/ae_marker_index.hpp>

namespace yas::ae {
class project_action_sender;

struct marker_element_controller final {
    [[nodiscard]] static std::shared_ptr<marker_element_controller> make_shared(window_lifetime_id const &);

    marker_element_controller(std::shared_ptr<project_action_sender> const &action_sender);

    void select_marker_at(marker_index const &);
    void toggle_marker_selection_at(marker_index const &);
    void begin_marker_renaming_at(marker_index const &);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;

    marker_element_controller(marker_element_controller const &) = delete;
    marker_element_controller(marker_element_controller &&) = delete;
    marker_element_controller &operator=(marker_element_controller const &) = delete;
    marker_element_controller &operator=(marker_element_controller &&) = delete;
};
}  // namespace yas::ae
