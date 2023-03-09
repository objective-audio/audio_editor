//
//  ae_display_space.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_display_space_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct display_space final {
    display_space();

    void set_view_region(ui::region const);
    void set_scale(ui::size const);

    ui::region region() const;
    observing::syncable observe(std::function<void(display_space_event const &)> &&);

    ui::size const &scale() const;

    std::optional<time::range> frame_range(uint32_t const sample_rate, frame_index_t const current_frame) const;

   private:
    ui::region _region;
    ui::region _view_region;
    ui::size _scale;
    observing::fetcher_ptr<display_space_event> _fetcher;

    observing::canceller_pool _pool;

    display_space(display_space const &) = delete;
    display_space(display_space &&) = delete;
    display_space &operator=(display_space const &) = delete;
    display_space &operator=(display_space &&) = delete;

    void _update_region_and_notify(display_space_event_source const);
};
}  // namespace yas::ae
