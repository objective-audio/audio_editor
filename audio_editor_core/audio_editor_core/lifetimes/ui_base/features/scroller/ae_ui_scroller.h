//
//  ae_ui_scroller.h
//

#pragma once

#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class scroller_presenter;
class scroll_gesture_controller;
class ui_track;
class ui_edge;
class ui_markers;

struct ui_scroller final {
    [[nodiscard]] static std::shared_ptr<ui_scroller> make_shared(window_lifetime_id const &,
                                                                  std::shared_ptr<ui::standard> const &,
                                                                  std::shared_ptr<ui_track> const &,
                                                                  std::shared_ptr<ui_edge> const &,
                                                                  std::shared_ptr<ui_markers> const &);

    ui_scroller(std::shared_ptr<ui::standard> const &, std::shared_ptr<scroller_presenter> const &,
                std::shared_ptr<scroll_gesture_controller> const &, std::shared_ptr<ui_track> const &,
                std::shared_ptr<ui_edge> const &, std::shared_ptr<ui_markers> const &);

    std::shared_ptr<ui::node> const node;

   private:
    std::shared_ptr<scroller_presenter> const _presenter;
    std::shared_ptr<scroll_gesture_controller> const _scroll_gesture_controller;
    std::shared_ptr<ui::node> const _scrolling_node;
    std::shared_ptr<ui_track> const _track;
    std::shared_ptr<ui_edge> const _edge;
    std::shared_ptr<ui_markers> const _markers;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
