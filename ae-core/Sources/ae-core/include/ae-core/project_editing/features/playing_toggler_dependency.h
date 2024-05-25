//
//  playing_toggler_dependency.h
//

#pragma once

namespace yas::ae {
struct player_for_playing_toggler {
    virtual ~player_for_playing_toggler() = default;

    virtual void set_playing(bool const) = 0;
    [[nodiscard]] virtual bool is_playing() const = 0;
};
}  // namespace yas::ae
