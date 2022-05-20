//
//  ae_display_space.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <audio_editor_core/ae_display_space_types.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct display_space final {
    [[nodiscard]] static std::shared_ptr<display_space> make_shared(ui::region const view_region);

    void set_view_region(ui::region const);
    void set_scale(ui::size const);

    ui::region region() const;
    observing::syncable observe(std::function<void(display_space_event const &)> &&);

    ui::size const &scale() const;

    time::range frame_range(uint32_t const sample_rate, frame_index_t const current_frame) const;

   private:
    ui::region _region;
    ui::region _view_region;
    ui::size _scale;
    observing::fetcher_ptr<display_space_event> _fetcher;

    observing::canceller_pool _pool;

    display_space(ui::region const view_region);

    void _update_region_and_notify(display_space_event_source const);
};
}  // namespace yas::ae
