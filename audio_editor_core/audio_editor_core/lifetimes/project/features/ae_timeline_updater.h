//
//  ae_timeline_updater.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_track;
class timeline_holder;

struct timeline_updater final {
    [[nodiscard]] static std::shared_ptr<timeline_updater> make_shared(file_track *, timeline_holder *);

    timeline_updater(file_track *, timeline_holder *);

   private:
    file_track *_file_track;
    timeline_holder *_timeline_holder;

    observing::canceller_pool _pool;
};
}  // namespace yas::ae
