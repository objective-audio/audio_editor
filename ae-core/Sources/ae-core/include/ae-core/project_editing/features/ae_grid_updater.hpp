//
//  ae_grid_updater.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_common_types.h>
#include <ae-core/project_editing/features/ae_grid_content_pool.h>

#include <ae-core/project_editing/value_types/ae_grid_content.hpp>
#include <observing/umbrella.hpp>

namespace yas::ae {
class timing;
class nudging;
class app_settings;

struct grid_updater final {
    grid_updater(timing *, nudging *, app_settings *, grid_content_pool *);

    struct source {
        frame_index_t begin_frame;
        frame_index_t end_frame;
        length_t min_interval = 0;

        bool operator==(source const &) const;
        bool operator!=(source const &) const;
    };

    void update_if_source_changed(source const &);
    void reset();

    [[nodiscard]] observing::syncable observe(std::function<void(void)> &&);

   private:
    timing *const _timing;
    nudging *const _nudging;
    app_settings *const _app_settings;
    grid_content_pool *const _content_pool;

    std::optional<source> _last_source;

    observing::canceller_pool _pool;

    void _update_if_source_is_available();
    void _update(source const &source);
};
}  // namespace yas::ae
