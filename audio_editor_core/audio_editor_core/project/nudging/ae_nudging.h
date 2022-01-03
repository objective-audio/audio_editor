//
//  ae_nudging.h
//

#pragma once

#include <audio_editor_core/ae_nudging_types.h>
#include <audio_editor_core/ae_project_editor_dependency.h>
#include <processing/yas_processing_common_types.h>

namespace yas::ae {
struct nudging final : nudging_for_project_editor {
    [[nodiscard]] static std::shared_ptr<nudging> make_shared(proc::sample_rate_t const);

    void set_kind(nudging_kind const);
    nudging_kind kind() const;

    void set_unit_count(uint32_t const);
    uint32_t unit_count() const;

    uint32_t sample_count() const;

   private:
    proc::sample_rate_t const _sample_rate;
    nudging_kind _kind;
    uint32_t _unit_count;

    nudging(proc::sample_rate_t const);

    uint32_t _sample_count_for_kind() const;
};
}  // namespace yas::ae
