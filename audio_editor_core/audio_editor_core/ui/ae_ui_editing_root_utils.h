//
//  ae_ui_root_utils.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_editing_root_presenter_types.h>
#include <audio_editor_core/ae_keyboard_types.h>
#include <ui/yas_ui_rgb_color.h>

namespace yas::ae::ui_editing_root_utils {
ui::color to_playing_line_color(playing_line_state_t const, std::shared_ptr<ae::color> const &);
}  // namespace yas::ae::ui_editing_root_utils
