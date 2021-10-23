//
//  ae_scrolling_types.h
//

#pragma once

namespace yas::ae {
enum scrolling_state {
    began,
    changed,
    ended,
};

struct scrolling_event {
    scrolling_state state;
    double delta_time;
};
}  // namespace yas::ae
