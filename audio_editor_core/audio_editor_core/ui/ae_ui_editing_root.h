//
//  ae_ui_editing_root.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_ui_button.h>
#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class editing_root_presenter;
class action_controller;
class pinch_gesture_controller;
class ui_track;
class ui_time;
class color;
class keyboard;

struct ui_editing_root final {
    std::shared_ptr<ui::node> const node;

    bool responds_to_action(action const);

    [[nodiscard]] static std::shared_ptr<ui_editing_root> make_shared(ui_project_id const &project_id,
                                                                      std::shared_ptr<ui_track> const &,
                                                                      std::shared_ptr<ui_time> const &);

   private:
    std::shared_ptr<editing_root_presenter> const _presenter;
    std::shared_ptr<action_controller> const _action_controller;
    std::weak_ptr<pinch_gesture_controller> const _pinch_gesture_controller;

    std::shared_ptr<ae::color> const _color;
    std::shared_ptr<ui::font_atlas> const _font_atlas;
    std::shared_ptr<ui::strings> const _status_strings;
    std::shared_ptr<ui::strings> const _file_info_strings;
    std::shared_ptr<ui::strings> const _file_track_strings;
    std::shared_ptr<ui::strings> const _marker_pool_strings;
    std::shared_ptr<ui_track> const _track;
    std::shared_ptr<ui::rect_plane> const _playing_line;
    std::shared_ptr<ui_time> const _time;

    observing::canceller_pool _pool;

    ui_editing_root(std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::font_atlas> const &font_atlas,
                    std::shared_ptr<ae::color> const &, std::shared_ptr<editing_root_presenter> const &,
                    std::shared_ptr<action_controller> const &, std::shared_ptr<pinch_gesture_controller> const &,
                    std::shared_ptr<ae::keyboard> const &, std::shared_ptr<ui_track> const &,
                    std::shared_ptr<ui_time> const &);

    void _setup_node_hierarchie();
    void _setup_observing(std::shared_ptr<ui::standard> const &, std::shared_ptr<ae::keyboard> const &);
    void _setup_layout(std::shared_ptr<ui::view_look> const &);
};
}  // namespace yas::ae
