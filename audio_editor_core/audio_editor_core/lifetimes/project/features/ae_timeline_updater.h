//
//  ae_timeline_updater.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_track;
class timeline_holder;

struct timeline_updater final {
    timeline_updater(file_track *, timeline_holder *);

   private:
    file_track *_file_track;
    timeline_holder *_timeline_holder;

    observing::canceller_pool _pool;

    timeline_updater(timeline_updater const &) = delete;
    timeline_updater(timeline_updater &&) = delete;
    timeline_updater &operator=(timeline_updater const &) = delete;
    timeline_updater &operator=(timeline_updater &&) = delete;
};
}  // namespace yas::ae
