//
//  ae_ui_playing_line_utils.h
//

#pragma once

#include <audio_editor_core/ae_action.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_key.h>
#include <audio_editor_core/ae_playing_line_state.h>
#include <ui/yas_ui_rgb_color.h>

namespace yas::ae::ui_playing_line_utils {
ui::color to_playing_line_color(playing_line_state_t const, ae::color *);
}  // namespace yas::ae::ui_playing_line_utils
