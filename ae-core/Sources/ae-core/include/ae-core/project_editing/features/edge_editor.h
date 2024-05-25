//
//  edge_editor.h
//

#pragma once

#include <memory>

namespace yas::ae {
class edge_holder;
class player;
class editing_status;

struct edge_editor final {
    edge_editor(edge_holder *, player const *, editing_status const *);

    [[nodiscard]] bool can_set_begin() const;
    [[nodiscard]] bool can_set_end() const;
    void set_begin();
    void set_end();

   private:
    edge_holder *const _holder;
    player const *const _player;
    editing_status const *const _editing_status;

    edge_editor(edge_editor const &) = delete;
    edge_editor(edge_editor &&) = delete;
    edge_editor &operator=(edge_editor const &) = delete;
    edge_editor &operator=(edge_editor &&) = delete;
};
}  // namespace yas::ae
