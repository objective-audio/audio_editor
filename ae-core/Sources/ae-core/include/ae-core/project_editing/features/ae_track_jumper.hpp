//
//  ae_track_jumper.hpp
//

#pragma once

namespace yas::ae {
class module_pool;
class vertical_scrolling;

struct track_jumper final {
    track_jumper(module_pool const *, vertical_scrolling *);

    [[nodiscard]] bool can_jump_to_beginning() const;
    [[nodiscard]] bool can_jump_to_end() const;

    void jump_to_beginning();
    void jump_to_end();

   private:
    module_pool const *const _module_pool;
    vertical_scrolling *const _vertical_scrolling;

    track_jumper(track_jumper const &) = delete;
    track_jumper(track_jumper &&) = delete;
    track_jumper &operator=(track_jumper const &) = delete;
    track_jumper &operator=(track_jumper &&) = delete;
};
}  // namespace yas::ae
