//
//  ae_nudge_settings.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct nudge_settings final {
    nudge_settings(timing_for_nudge_settings const *);

    void rotate_next_unit();
    void rotate_previous_unit();
    [[nodiscard]] std::size_t unit_index() const;
    [[nodiscard]] observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&);

    [[nodiscard]] frame_index_t next_nudging_frame(frame_index_t const current_frame,
                                                   uint32_t const offset_count) const;
    [[nodiscard]] frame_index_t previous_nudging_frame(frame_index_t const current_frame,
                                                       uint32_t const offset_count) const;

    [[nodiscard]] frame_index_t current_grid_frame(frame_index_t const current_frame) const;
    [[nodiscard]] frame_index_t next_grid_frame(frame_index_t const current_frame) const;

   private:
    timing_for_nudge_settings const *const _timing;
    observing::value::holder_ptr<std::size_t> const _unit_idx;

    nudge_settings(nudge_settings const &) = delete;
    nudge_settings(nudge_settings &&) = delete;
    nudge_settings &operator=(nudge_settings const &) = delete;
    nudge_settings &operator=(nudge_settings &&) = delete;
};
}  // namespace yas::ae
