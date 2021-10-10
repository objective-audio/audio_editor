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
        case key::zero:
            return action::return_to_zero;
    }
}
