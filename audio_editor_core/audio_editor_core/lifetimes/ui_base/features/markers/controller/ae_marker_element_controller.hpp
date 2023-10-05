//
//  ae_marker_element_controller.hpp
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_types.h>

#include <audio_editor_core/ae_marker_index.hpp>

namespace yas::ae {
class project_action_sender;
class range_selector;
class deselector;

struct marker_element_controller final {
    [[nodiscard]] static std::shared_ptr<marker_element_controller> make_shared(project_lifetime_id const &);

    marker_element_controller(std::shared_ptr<project_action_sender> const &, std::shared_ptr<range_selector> const &,
                              std::shared_ptr<deselector> const &);

    void deselect_all();
    void begin_range_selection(ui::point const &);

    void toggle_selection(marker_index const &);
    void begin_renaming(marker_index const &);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<range_selector> const _range_selector;
    std::weak_ptr<deselector> const _deselector;

    marker_element_controller(marker_element_controller const &) = delete;
    marker_element_controller(marker_element_controller &&) = delete;
    marker_element_controller &operator=(marker_element_controller const &) = delete;
    marker_element_controller &operator=(marker_element_controller &&) = delete;
};
}  // namespace yas::ae
