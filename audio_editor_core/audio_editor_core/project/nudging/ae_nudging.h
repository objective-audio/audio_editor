//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_dependency.h>
#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct nudging final : nudging_for_project_editor {
    [[nodiscard]] static std::shared_ptr<nudging> make_shared(std::shared_ptr<timing_for_nudging> const &);

    void rotate_next_unit() override;
    void rotate_previous_unit() override;
    [[nodiscard]] std::size_t unit_index() const override;
    [[nodiscard]] observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&) override;

    void set_offset_count(uint32_t const);
    [[nodiscard]] uint32_t offset_count() const;

    [[nodiscard]] frame_index_t next_frame(frame_index_t const) const override;
    [[nodiscard]] frame_index_t previous_frame(frame_index_t const) const override;

   private:
    std::shared_ptr<timing_for_nudging> const _timing;
    observing::value::holder_ptr<std::size_t> const _unit_idx;
    uint32_t _offset_count;

    nudging(std::shared_ptr<timing_for_nudging> const &);
};
}  // namespace yas::ae
