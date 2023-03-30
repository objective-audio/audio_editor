//
//  ae_nudge_settings.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct nudge_settings final {
    nudge_settings(timing_for_nudge_settings const *, app_settings_for_nudge_settings *);

    [[nodiscard]] timing_unit_kind kind() const;

    void rotate_next_unit();
    void rotate_previous_unit();
    [[nodiscard]] std::size_t unit_index() const;
    [[nodiscard]] observing::syncable observe_kind(std::function<void(timing_unit_kind const &)> &&);

    [[nodiscard]] frame_index_t next_nudging_frame(frame_index_t const current_frame,
                                                   uint32_t const offset_count) const;
    [[nodiscard]] frame_index_t previous_nudging_frame(frame_index_t const current_frame,
                                                       uint32_t const offset_count) const;

    [[nodiscard]] frame_index_t current_grid_frame(frame_index_t const current_frame) const;
    [[nodiscard]] frame_index_t next_grid_frame(frame_index_t const current_frame) const;

   private:
    timing_for_nudge_settings const *const _timing;
    app_settings_for_nudge_settings *const _app_settings;
    observing::value::holder_ptr<timing_unit_kind> const _kind;

    nudge_settings(nudge_settings const &) = delete;
    nudge_settings(nudge_settings &&) = delete;
    nudge_settings &operator=(nudge_settings const &) = delete;
    nudge_settings &operator=(nudge_settings &&) = delete;

    void _update_app_settings();
};
}  // namespace yas::ae
