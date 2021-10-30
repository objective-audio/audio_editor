//
//  ae_ui_root_utils.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <ui/yas_ui_color.h>

namespace yas::ae::ui_editing_root_utils {
action to_action(key const);
ui::color to_playing_line_color(playing_line_state_t const);
}  // namespace yas::ae::ui_editing_root_utils
