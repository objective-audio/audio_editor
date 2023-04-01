//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependencies.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct nudging final {
    nudging(timing_for_nudging const *, app_settings_for_nudging *);

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
    timing_for_nudging const *const _timing;
    app_settings_for_nudging *const _app_settings;
    observing::value::holder_ptr<timing_unit_kind> const _kind;

    nudging(nudging const &) = delete;
    nudging(nudging &&) = delete;
    nudging &operator=(nudging const &) = delete;
    nudging &operator=(nudging &&) = delete;

    void _update_app_settings();
};
}  // namespace yas::ae
