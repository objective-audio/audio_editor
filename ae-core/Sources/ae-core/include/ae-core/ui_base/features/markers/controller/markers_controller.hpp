//
//  markers_controller.hpp
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ui/common/yas_ui_types.h>

#include <ae-core/project_editing/value_types/marker_index.hpp>
#include <set>

namespace yas::ae {
class project_action_sender;
class range_selector;
class deselector;

struct markers_controller final {
    [[nodiscard]] static std::shared_ptr<markers_controller> make_shared(project_lifetime_id const &);

    markers_controller(std::shared_ptr<project_action_sender> const &, std::shared_ptr<range_selector> const &,
                       std::shared_ptr<deselector> const &);

    void deselect_all();
    void begin_range_selection(ui::point const &);

    void begin_selection();
    void select(std::set<marker_index> const &);
    void end_selection();
    void toggle_selection(marker_index const &);
    void begin_renaming(marker_index const &);

   private:
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<range_selector> const _range_selector;
    std::weak_ptr<deselector> const _deselector;

    markers_controller(markers_controller const &) = delete;
    markers_controller(markers_controller &&) = delete;
    markers_controller &operator=(markers_controller const &) = delete;
    markers_controller &operator=(markers_controller &&) = delete;
};
}  // namespace yas::ae
