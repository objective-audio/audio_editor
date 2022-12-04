//
//  ae_database_updater.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class edge_holder;
class pasteboard;
class database;

struct database_updater final {
    database_updater(edge_holder *, database *);

   private:
    edge_holder *const _edge_holder;
    database *const _database;

    observing::canceller_pool _pool;

    void _setup();
};
}  // namespace yas::ae
