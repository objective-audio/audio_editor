//
//  ae_nudger.h
//

#pragma once

#include <memory>

namespace yas::ae {
class player;
class nudging;

struct nudger final {
    nudger(player *, nudging *);

    [[nodiscard]] bool can_nudge() const;
    void nudge_previous(uint32_t const offset_count);
    void nudge_next(uint32_t const offset_count);

   private:
    player *const _player;
    nudging const *const _nudging;

    nudger(nudger const &) = delete;
    nudger(nudger &&) = delete;
    nudger &operator=(nudger const &) = delete;
    nudger &operator=(nudger &&) = delete;
};
}  // namespace yas::ae
