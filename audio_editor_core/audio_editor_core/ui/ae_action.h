//
//  ae_action.h
//

#pragma once

namespace yas::ae {
enum class action {
    toggle_play,
    nudge_previous,
    nudge_next,
    jump_previous,
    jump_next,
    drop_head_and_offset,
    split,
    drop_tail_and_offset,
    erase,
    return_to_zero,
};
}
