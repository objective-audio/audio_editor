//
//  ui_playing_line_utils.cpp
//

#include "ui_playing_line_utils.h"

using namespace yas;
using namespace yas::ae;

ui::color ui_playing_line_utils::to_playing_line_color(playing_line_state_t const state, ae::color *color) {
    switch (state) {
        case playing_line_state_t::playing:
            return color->playing_line_playing();
        case playing_line_state_t::scrolling:
            return color->playing_line_scrolling();
        case playing_line_state_t::pausing:
            return color->playing_line_pausing();
    }
}
