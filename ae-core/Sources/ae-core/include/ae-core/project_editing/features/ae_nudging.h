//
//  ae_nudging.h
//

#pragma once

#include <ae-core/project_editing/features/ae_nudging_dependencies.h>

namespace yas::ae {
struct nudging final {
    nudging(timing_for_nudging const *, app_settings_for_nudging *);

    void rotate_next_unit();
    void rotate_previous_unit();

    [[nodiscard]] frame_index_t next_nudging_frame(frame_index_t const current_frame,
                                                   uint32_t const offset_count) const;
    [[nodiscard]] frame_index_t previous_nudging_frame(frame_index_t const current_frame,
                                                       uint32_t const offset_count) const;

    [[nodiscard]] frame_index_t current_grid_frame(frame_index_t const current_frame) const;
    [[nodiscard]] frame_index_t next_grid_frame(frame_index_t const current_frame) const;

   private:
    timing_for_nudging const *const _timing;
    app_settings_for_nudging *const _app_settings;

    nudging(nudging const &) = delete;
    nudging(nudging &&) = delete;
    nudging &operator=(nudging const &) = delete;
    nudging &operator=(nudging &&) = delete;

    std::size_t _unit_index() const;
};
}  // namespace yas::ae
