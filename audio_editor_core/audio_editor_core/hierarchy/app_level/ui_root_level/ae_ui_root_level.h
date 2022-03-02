//
//  ui_root_level.h
//

#pragma once

#include <audio_editor_core/ae_ui_project_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class display_space;
class keyboard;
class pinch_gesture_controller;
class ui_editing_root_level_pool;
class ui_root;

struct ui_root_level {
    [[nodiscard]] static std::shared_ptr<ui_root_level> make_shared(std::shared_ptr<ui::standard> const &,
                                                                    ui_project_id const &project_id);
    ui_project_id const project_id;

    std::shared_ptr<ui::standard> const standard;
    std::shared_ptr<ui::texture> const texture;
    std::shared_ptr<ui::font_atlas> const font_atlas_14;
    std::shared_ptr<ae::display_space> const display_space;
    std::shared_ptr<ae::keyboard> const keyboard;
    std::shared_ptr<ae::pinch_gesture_controller> const pinch_gesture_controller;
    std::shared_ptr<ui_editing_root_level_pool> const editing_root_level_pool;
    std::shared_ptr<ui_root> const root;

   private:
    ui_root_level(std::shared_ptr<ui::standard> const &, ui_project_id const &project_id);
};
}  // namespace yas::ae
