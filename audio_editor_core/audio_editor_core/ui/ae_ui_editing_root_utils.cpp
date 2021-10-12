//
//  ae_ui_root_utils.cpp
//

#include "ae_ui_editing_root_utils.h"

using namespace yas;
using namespace yas::ae;

action ui_editing_root_utils::to_action(key const key) {
    switch (key) {
        case key::space:
            return action::toggle_play;
        case key::del:
            return action::erase_and_offset;
        case key::a:
            return action::drop_head_and_offset;
        case key::s:
            return action::split;
        case key::d:
            return action::drop_tail_and_offset;
        case key::num_0:
            return action::return_to_zero;
        case key::num_1:
            return action::go_to_marker_1;
        case key::num_2:
            return action::go_to_marker_2;
        case key::num_3:
            return action::go_to_marker_3;
        case key::num_4:
            return action::go_to_marker_4;
        case key::num_5:
            return action::go_to_marker_5;
        case key::num_6:
            return action::go_to_marker_6;
        case key::num_7:
            return action::go_to_marker_7;
        case key::num_8:
            return action::go_to_marker_8;
        case key::num_9:
            return action::go_to_marker_9;
    }
}
