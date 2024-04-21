//
//  ae_ui_playing_line_utils.h
//

#pragma once

#include <ae-core/app/features/ae_color.h>
#include <ae-core/global/value_types/ae_action.h>
#include <ae-core/ui_base/features/playing_line/presenter/ae_playing_line_state.h>
#include <ae-core/ui_resource/features/ae_key.h>
#include <ui/color/yas_ui_rgb_color.h>

namespace yas::ae::ui_playing_line_utils {
ui::color to_playing_line_color(playing_line_state_t const, ae::color *);
}  // namespace yas::ae::ui_playing_line_utils
