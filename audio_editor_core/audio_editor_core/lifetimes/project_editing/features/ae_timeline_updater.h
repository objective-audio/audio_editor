//
//  ae_timeline_updater.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class module_pool;
class timeline_holder;

struct timeline_updater final {
    timeline_updater(module_pool *, timeline_holder *);

   private:
    module_pool *_module_pool;
    timeline_holder *_timeline_holder;

    observing::canceller_pool _pool;

    timeline_updater(timeline_updater const &) = delete;
    timeline_updater(timeline_updater &&) = delete;
    timeline_updater &operator=(timeline_updater const &) = delete;
    timeline_updater &operator=(timeline_updater &&) = delete;
};
}  // namespace yas::ae
