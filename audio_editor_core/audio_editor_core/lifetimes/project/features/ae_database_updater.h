//
//  ae_database_updater.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_track;
class marker_pool;
class edge_holder;
class pasteboard;
class database;

struct database_updater final {
    [[nodiscard]] static std::shared_ptr<database_updater> make_shared(file_track *, edge_holder *, pasteboard *,
                                                                       database *);

    database_updater(file_track *, edge_holder *, pasteboard *, database *);

   private:
    file_track *const _file_track;
    edge_holder *const _edge_holder;
    pasteboard *const _pasteboard;
    database *const _database;

    observing::canceller_pool _pool;

    void _setup();
};
}  // namespace yas::ae
