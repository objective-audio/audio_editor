//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct nudging final : nudging_for_project_editor {
    [[nodiscard]] static std::shared_ptr<nudging> make_shared(sample_rate_t const);

    void set_kind(nudging_kind const) override;
    [[nodiscard]] nudging_kind kind() const override;
    [[nodiscard]] observing::syncable observe_kind(std::function<void(nudging_kind const &)> &&) override;

    void set_unit_count(uint32_t const);
    [[nodiscard]] uint32_t unit_count() const;

    [[nodiscard]] uint32_t unit_sample_count() const override;

   private:
    sample_rate_t const _sample_rate;
    observing::value::holder_ptr<nudging_kind> const _kind;
    uint32_t _unit_count;

    nudging(sample_rate_t const);
};
}  // namespace yas::ae
