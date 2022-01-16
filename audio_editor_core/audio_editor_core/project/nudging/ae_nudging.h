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

    void set_kind(nudging_kind const) override;
    [[nodiscard]] nudging_kind kind() const override;
    [[nodiscard]] observing::syncable observe_kind(std::function<void(nudging_kind const &)> &&) override;

    void set_offset_count(uint32_t const);
    [[nodiscard]] uint32_t offset_count() const;

    [[nodiscard]] frame_index_t next_frame(frame_index_t const) const override;
    [[nodiscard]] frame_index_t previous_frame(frame_index_t const) const override;

   private:
    std::shared_ptr<timing_for_nudging> const _timing;
    observing::value::holder_ptr<nudging_kind> const _kind;
    uint32_t _unit_count;

    nudging(std::shared_ptr<timing_for_nudging> const &);
};
}  // namespace yas::ae
