//
//  ae_ui_editing_root.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class editing_root_presenter;
class project_action_sender;
class pinch_gesture_controller;
class ui_track;
class ui_scroller;
class ui_modal_bg;
class ui_time;
class color;
class keyboard;

struct ui_editing_root final {
    std::shared_ptr<ui::node> const node;

    [[nodiscard]] static std::shared_ptr<ui_editing_root> make_shared(
        window_lifetime_id const &, std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::font_atlas> const &,
        std::shared_ptr<ae::keyboard> const &, std::shared_ptr<ui_scroller> const &,
        std::shared_ptr<ui_modal_bg> const &, std::shared_ptr<ui_time> const &);

    ui_editing_root(std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::font_atlas> const &font_atlas,
                    std::shared_ptr<ae::color> const &, std::shared_ptr<editing_root_presenter> const &,
                    std::shared_ptr<project_action_sender> const &, std::shared_ptr<pinch_gesture_controller> const &,
                    std::shared_ptr<ae::keyboard> const &, std::shared_ptr<ui_scroller> const &,
                    std::shared_ptr<ui_modal_bg> const &, std::shared_ptr<ui_time> const &);

    bool responds_to_action(action const);

   private:
    std::shared_ptr<editing_root_presenter> const _presenter;
    std::weak_ptr<project_action_sender> const _action_sender;
    std::weak_ptr<pinch_gesture_controller> const _pinch_gesture_controller;

    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::rect_plane> const _playing_line;
    std::shared_ptr<ui_scroller> const _scroller;
    std::shared_ptr<ui_modal_bg> const _modal_bg;
    std::shared_ptr<ui_time> const _time;

    observing::canceller_pool _pool;

    void _setup_node_hierarchie();
    void _setup_observing(std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::keyboard> const &);
    void _setup_layout(std::shared_ptr<ui::view_look> const &);
};
}  // namespace yas::ae
