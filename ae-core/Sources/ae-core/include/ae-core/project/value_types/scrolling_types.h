//
//  scrolling_types.h
//

#pragma once

namespace yas::ae {
enum class scrolling_state {
    began,
    changed,
    ended,
};

struct scrolling_event final {
    scrolling_state state;
    double delta_time;
};
}  // namespace yas::ae
