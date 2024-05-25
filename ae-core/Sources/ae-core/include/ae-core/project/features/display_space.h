//
//  display_space.h
//

#pragma once

#include <ae-core/global/value_types/common_types.h>
#include <ae-core/project/features/display_space_types.h>

#include <ae-core/global/value_types/track_range.hpp>
#include <observing/umbrella.hpp>

namespace yas::ae {
struct display_space final {
    display_space();

    void set_view_region(ui::region const);
    void set_scale(ui::size const);

    [[nodiscard]] ui::region region() const;
    [[nodiscard]] observing::syncable observe(std::function<void(display_space_event const &)> &&);

    [[nodiscard]] ui::region const &view_region() const;
    [[nodiscard]] ui::size const &scale() const;

    [[nodiscard]] std::optional<time::range> frame_range(uint32_t const sample_rate,
                                                         frame_index_t const current_frame) const;
    [[nodiscard]] float frames_per_point_width(uint32_t const sample_rate) const;

    [[nodiscard]] std::optional<track_range> track_range(double const track_offset) const;

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
