//
//  ae_marker_editor.h
//

#pragma once

#include <memory>

namespace yas::ae {
class player;
class marker_pool;
class database;
class editing_status;

struct marker_editor final {
    [[nodiscard]] static std::shared_ptr<marker_editor> make_shared(player const *, marker_pool *, database *,
                                                                    editing_status const *);

    marker_editor(player const *, marker_pool *, database *, editing_status const *);

    [[nodiscard]] bool can_insert_marker() const;
    void insert_marker();

   private:
    player const *const _player;
    marker_pool *const _marker_pool;
    database *const _database;
    editing_status const *const _editing_status;
};
}  // namespace yas::ae
