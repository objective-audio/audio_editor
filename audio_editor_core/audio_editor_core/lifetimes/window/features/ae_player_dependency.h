//
//  ae_player_dependency.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::playing {
class renderer;
class coordinator;
}  // namespace yas::playing

namespace yas::audio {
class io_device;
}

namespace yas::ae {
class scrolling_event;

struct scrolling_for_player {
    virtual ~scrolling_for_player() = default;

    [[nodiscard]] virtual bool is_began() const = 0;
    virtual void set_is_enabled(bool const) = 0;

    [[nodiscard]] virtual observing::endable observe(std::function<void(scrolling_event const &)> &&) = 0;
};
}  // namespace yas::ae
