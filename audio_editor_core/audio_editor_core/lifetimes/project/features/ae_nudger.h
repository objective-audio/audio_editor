//
//  ae_nudger.h
//

#pragma once

#include <memory>

namespace yas::ae {
class player;
class nudge_settings;

struct nudger final {
    nudger(player *, nudge_settings *);

    [[nodiscard]] bool can_nudge() const;
    void nudge_previous(uint32_t const offset_count);
    void nudge_next(uint32_t const offset_count);

   private:
    player *const _player;
    nudge_settings const *const _settings;
};
}  // namespace yas::ae
