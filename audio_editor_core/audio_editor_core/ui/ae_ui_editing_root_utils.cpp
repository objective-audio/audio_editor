//
//  ae_ui_root_utils.cpp
//

#include "ae_ui_editing_root_utils.h"

using namespace yas;
using namespace yas::ae;

ui::color ui_editing_root_utils::to_playing_line_color(playing_line_state_t const state) {
    switch (state) {
        case playing_line_state_t::playing:
            return ui::green_color();
        case playing_line_state_t::scrolling:
            return ui::orange_color();
        case playing_line_state_t::nothing:
            return ui::light_gray_color();
    }
}
