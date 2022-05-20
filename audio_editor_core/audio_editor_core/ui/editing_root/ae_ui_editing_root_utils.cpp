//
//  ae_ui_root_utils.cpp
//

#include "ae_ui_editing_root_utils.h"

using namespace yas;
using namespace yas::ae;

ui::color ui_editing_root_utils::to_playing_line_color(playing_line_state_t const state,
                                                       std::shared_ptr<ae::color> const &color) {
    switch (state) {
        case playing_line_state_t::playing:
            return color->playing_line_playing();
        case playing_line_state_t::scrolling:
            return color->playing_line_scroling();
        case playing_line_state_t::pausing:
            return color->playing_line_pausing();
    }
}
