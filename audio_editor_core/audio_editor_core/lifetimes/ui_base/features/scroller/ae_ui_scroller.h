//
//  ae_ui_scroller.h
//

#pragma once

#include <audio_editor_core/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

#include <audio_editor_core/ae_ui_scroller_nodes.hpp>

namespace yas::ae {
class scroller_presenter;
class scroll_gesture_controller;
class ui_zooming;
class ui_edge;
class ui_markers;
class ui_scroller_nodes;

struct ui_scroller final {
    [[nodiscard]] static std::shared_ptr<ui_scroller> make_shared(project_lifetime_id const &,
                                                                  ui_scroller_nodes const &scroller_nodes);

    ui_scroller(std::shared_ptr<ui::standard> const &, ui_scroller_nodes const &scroller_nodes,
                std::shared_ptr<scroller_presenter> const &, std::shared_ptr<scroll_gesture_controller> const &);

   private:
    std::shared_ptr<scroller_presenter> const _presenter;
    std::shared_ptr<scroll_gesture_controller> const _scroll_gesture_controller;
    ui::node *const _back_node;
    ui::node *const _modules_node;
    ui::node *const _front_y_fixed_node;
    ui::node *const _front_x_fixed_node;

    observing::canceller_pool _pool;

    ui_scroller(ui_scroller const &) = delete;
    ui_scroller(ui_scroller &&) = delete;
    ui_scroller &operator=(ui_scroller const &) = delete;
    ui_scroller &operator=(ui_scroller &&) = delete;
};
}  // namespace yas::ae
