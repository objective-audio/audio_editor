//
//  ae_ui_root_level.mm
//

#include "ae_ui_root_level.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_keyboard.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_ui_editing_root_level_pool.h>
#include <audio_editor_core/ae_ui_root.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root_level> ui_root_level::make_shared(std::shared_ptr<ui::standard> const &standard,
                                                          ui_project_id const &project_id) {
    return std::shared_ptr<ui_root_level>(new ui_root_level{standard, project_id});
}

ui_root_level::ui_root_level(std::shared_ptr<ui::standard> const &standard, ui_project_id const &project_id)
    : project_id(project_id),
      standard(standard),
      texture(ui::texture::make_shared({.point_size = {1024, 1024}}, standard->view_look())),
      font_atlas_14(ui::font_atlas::make_shared(
          {.font_name = "TrebuchetMS-Bold",
           .font_size = 14.0f,
           .words = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:[]"},
          this->texture)),
      display_space(display_space::make_shared(standard->view_look()->view_layout_guide()->region())),
      keyboard(ae::keyboard::make_shared(standard->event_manager())),
      pinch_gesture_controller(pinch_gesture_controller::make_shared(project_id.identifier)),
      editing_root_level_pool(ui_editing_root_level_pool::make_shared(project_id)),
      root(ui_root::make_shared(standard, project_id, this->editing_root_level_pool)) {
}
