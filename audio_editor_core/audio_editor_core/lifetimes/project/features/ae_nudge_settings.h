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

    [[nodiscard]] std::optional<frame_index_t> next_frame(frame_index_t const current_frame,
                                                          uint32_t const offset_count) const;
    [[nodiscard]] std::optional<frame_index_t> previous_frame(frame_index_t const current_frame,
                                                              uint32_t const offset_count) const;

   private:
    timing_for_nudge_settings const *const _timing;
    observing::value::holder_ptr<std::size_t> const _unit_idx;
};
}  // namespace yas::ae
