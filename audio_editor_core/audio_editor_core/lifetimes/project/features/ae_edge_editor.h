//
//  ae_edge_editor.h
//

#pragma once

#include <memory>

namespace yas::ae {
class edge_holder;
class player;
class editing_status;
class database;

struct edge_editor final {
    edge_editor(edge_holder *, player const *, editing_status const *, database *);

    [[nodiscard]] bool can_set_begin() const;
    [[nodiscard]] bool can_set_end() const;
    void set_begin();
    void set_end();

   private:
    edge_holder *const _holder;
    player const *const _player;
    editing_status const *const _editing_status;
    database *const _database;
};
}  // namespace yas::ae
