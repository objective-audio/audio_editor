//
//  ae_reverter.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class database;
class file_track;
class marker_pool;
class pasteboard;
class edge_holder;
class editing_status;

struct reverter final {
    [[nodiscard]] static std::shared_ptr<reverter> make_shared(database *, file_track *, marker_pool *, pasteboard *,
                                                               edge_holder *, editing_status const *);

    reverter(database *, file_track *, marker_pool *, pasteboard *, edge_holder *, editing_status const *);

    [[nodiscard]] bool can_undo() const;
    void undo();

    [[nodiscard]] bool can_redo() const;
    void redo();

    [[nodiscard]] bool can_purge() const;
    void purge();

   private:
    database *const _database;
    file_track *const _file_track;
    marker_pool *const _marker_pool;
    pasteboard *const _pasteboard;
    edge_holder *const _edge_holder;
    editing_status const *const _editing_status;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
