//
//  ae_ui_root.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_ui_button.h>
#include <audio_editor_core/ae_ui_editing_root_dependency.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class editing_root_presenter;
class action_controller;
class pinch_gesture_controller;
class ui_track;
class ui_time;

struct ui_editing_root final {
    bool responds_to_action(action const);

    static std::shared_ptr<ui_editing_root> make_shared(std::shared_ptr<ui::standard> const &,
                                                        std::string const &project_id);

   private:
    std::shared_ptr<editing_root_presenter> const _presenter;
    std::weak_ptr<action_controller> const _action_controller;
    std::shared_ptr<pinch_gesture_controller> const _pinch_gesture_controller;

    std::shared_ptr<ui::standard> const _standard;
    std::shared_ptr<keyboard_for_ui_root> const _keyboard;
    std::shared_ptr<ui::font_atlas> const _font_atlas;
    std::shared_ptr<ui_button> const _play_button;
    std::shared_ptr<ui_button> const _split_button;
    std::shared_ptr<ui_button> const _drop_head_and_offset_button;
    std::shared_ptr<ui_button> const _drop_tail_and_offset_button;
    std::shared_ptr<ui_button> const _erase_and_offset_button;
    std::shared_ptr<ui_button> const _zero_button;
    std::shared_ptr<ui_button> const _jump_previous_button;
    std::shared_ptr<ui_button> const _jump_next_button;
    std::shared_ptr<ui_button> const _nudge_button;
    std::shared_ptr<ui_button> const _timing_button;
    std::shared_ptr<ui_button> const _insert_marker_button;
    std::shared_ptr<ui_button> const _undo_button;
    std::shared_ptr<ui_button> const _redo_button;
    std::shared_ptr<ui_button> const _export_button;
    std::vector<std::shared_ptr<ui_button>> const _buttons;
    std::shared_ptr<ui::collection_layout> const _button_collection_layout;
    std::shared_ptr<ui::strings> const _status_strings;
    std::shared_ptr<ui::strings> const _file_info_strings;
    std::shared_ptr<ui::strings> const _file_track_strings;
    std::shared_ptr<ui::strings> const _marker_pool_strings;
    std::shared_ptr<ui_track> const _track;
    std::shared_ptr<ui::rect_plane> const _playing_line;
    std::shared_ptr<ui_time> const _time;

    observing::canceller_pool _pool;

    ui_editing_root(std::shared_ptr<ui::standard> const &, std::shared_ptr<ui::texture> const &texture,
                    std::shared_ptr<editing_root_presenter> const &, std::shared_ptr<action_controller> const &,
                    std::shared_ptr<pinch_gesture_controller> const &, std::shared_ptr<ui_track> const &,
                    std::shared_ptr<ui_time> const &);

    void _setup_node_hierarchie();
    void _setup_observing();
    void _setup_layout();

    void _update_buttons_enabled();
};
}  // namespace yas::ae
