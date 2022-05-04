//
//  ae_nudge_settings.h
//

#pragma once

#include <audio_editor_core/ae_nudge_settings_dependency.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct nudge_settings final : nudge_settings_for_project_editor {
    [[nodiscard]] static std::shared_ptr<nudge_settings> make_shared(
        std::shared_ptr<timing_for_nudge_settings> const &);

    void rotate_next_unit() override;
    void rotate_previous_unit() override;
    [[nodiscard]] std::size_t unit_index() const override;
    [[nodiscard]] observing::syncable observe_unit_index(std::function<void(std::size_t const &)> &&) override;

    [[nodiscard]] std::optional<frame_index_t> next_frame(frame_index_t const current_frame,
                                                          uint32_t const offset_count) const override;
    [[nodiscard]] std::optional<frame_index_t> previous_frame(frame_index_t const current_frame,
                                                              uint32_t const offset_count) const override;

   private:
    std::weak_ptr<timing_for_nudge_settings> const _timing;
    observing::value::holder_ptr<std::size_t> const _unit_idx;

    nudge_settings(std::shared_ptr<timing_for_nudge_settings> const &);
};
}  // namespace yas::ae
