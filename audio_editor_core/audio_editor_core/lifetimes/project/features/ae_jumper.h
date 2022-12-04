//
//  ae_jumper.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_jumper_dependency.h>

#include <memory>
#include <optional>

namespace yas::ae {
class player;
class file_track;
class marker_pool;
class edge_holder;

struct jumper final {
    jumper(player *, file_track const *, marker_pool const *, edge_holder const *);

    [[nodiscard]] bool can_jump_to_previous_edge() const;
    [[nodiscard]] bool can_jump_to_next_edge() const;
    [[nodiscard]] bool can_jump_to_beginnig() const;
    [[nodiscard]] bool can_jump_to_end() const;
    void jump_to_previous_edge();
    void jump_to_next_edge();
    void jump_to_beginning();
    void jump_to_end();

    [[nodiscard]] bool can_return_to_zero() const;
    void return_to_zero();

    [[nodiscard]] bool can_go_to_marker(std::size_t const) const;
    void go_to_marker(std::size_t const);

   private:
    player *_player;
    file_track const *const _file_track;
    marker_pool const *const _marker_pool;
    edge_holder const *const _edge_holder;

    std::optional<frame_index_t> _previous_jumpable_frame() const;
    std::optional<frame_index_t> _next_jumpable_frame() const;
    std::optional<frame_index_t> _first_edge() const;
    std::optional<frame_index_t> _last_edge() const;
};
}  // namespace yas::ae
