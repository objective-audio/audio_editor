//
//  ae_jumper.h
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>
#include <ae-core/project_editing/features/ae_jumper_dependency.h>

#include <memory>
#include <optional>

namespace yas::ae {
class player;
class module_pool;
class marker_pool;
class edge_holder;

struct jumper final {
    jumper(player *, module_pool const *, marker_pool const *, edge_holder const *);

    [[nodiscard]] bool can_jump_to_previous_edge() const;
    [[nodiscard]] bool can_jump_to_next_edge() const;
    [[nodiscard]] bool can_jump_to_beginning() const;
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
    module_pool const *const _module_pool;
    marker_pool const *const _marker_pool;
    edge_holder const *const _edge_holder;

    jumper(jumper const &) = delete;
    jumper(jumper &&) = delete;
    jumper &operator=(jumper const &) = delete;
    jumper &operator=(jumper &&) = delete;

    std::optional<frame_index_t> _previous_jumpable_frame() const;
    std::optional<frame_index_t> _next_jumpable_frame() const;
    std::optional<frame_index_t> _first_edge() const;
    std::optional<frame_index_t> _last_edge() const;
};
}  // namespace yas::ae
