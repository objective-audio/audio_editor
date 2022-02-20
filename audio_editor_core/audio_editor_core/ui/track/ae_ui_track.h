//
//  ae_ui_track.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class track_presenter;
class ui_modules;
class ui_markers;
class ui_edge;
class scroll_gesture_controller;
class display_space;

struct ui_track final {
    [[nodiscard]] static std::shared_ptr<ui_track> make_shared(std::string const &project_id,
                                                               uintptr_t const project_view_id);

    std::shared_ptr<ui::node> const node() const;

   private:
    std::shared_ptr<track_presenter> const _presenter;
    std::shared_ptr<scroll_gesture_controller> const _scroll_gesture_controller;

    std::shared_ptr<ui::standard> const _standard;
    std::shared_ptr<display_space> const _display_space;
    std::shared_ptr<ui::node> const _root_node;
    std::shared_ptr<ui::node> const _time_node;
    std::shared_ptr<ui_modules> const _modules;
    std::shared_ptr<ui_edge> const _edge;
    std::shared_ptr<ui_markers> const _markers;

    observing::canceller_pool _pool;

    ui_track(std::shared_ptr<ui::standard> const &, std::shared_ptr<display_space> const &,
             std::shared_ptr<track_presenter> const &, std::shared_ptr<scroll_gesture_controller> const &,
             std::shared_ptr<ui_modules> const &, std::shared_ptr<ui_edge> const &,
             std::shared_ptr<ui_markers> const &);

    void _update_scale();
};
}  // namespace yas::ae
